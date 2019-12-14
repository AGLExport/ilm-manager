#include "CWindowManager.hpp"

#include <string>
#include <iostream>
#include <fstream>

#include <stdlib.h>


//-----------------------------------------------------------------------------
CWindowManager::CWindowManager()
{
	
	
}
//-----------------------------------------------------------------------------
CWindowManager::~CWindowManager()
{
	
}
//-----------------------------------------------------------------------------
CWindowManager CWindowManager::m_Win;

CWindowManager* CWindowManager::getInstance()
{
	return &m_Win;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void CWindowManager::ILMSurfaceCallbackFunctionStatic(t_ilm_uint id, struct ilmSurfaceProperties* sp, t_ilm_notification_mask m)
{
	CWindowManager *pwin = CWindowManager::getInstance();
	
	pwin->ILMSurfaceCallbackFunction(id, sp, m);
}
//-----------------------------------------------------------------------------
void CWindowManager::ILMCallBackFunctionStatic(ilmObjectType object, t_ilm_uint id, t_ilm_bool created, void *user_data)
{
	CWindowManager *pwin = CWindowManager::getInstance();
	
	pwin->ILMCallBackFunction(object, id, created);
}
//-----------------------------------------------------------------------------
void CWindowManager::ILMSurfaceCallbackFunction(t_ilm_uint id, struct ilmSurfaceProperties* sp, t_ilm_notification_mask m)
{
	printf("CWindowManager::ILMSurfaceCallbackFunction: surface (%d) created\n",id);
	
	if ((unsigned)m & ILM_NOTIFICATION_CONFIGURED)
	{
		this->HandlingSurface(id, sp->origSourceWidth, sp->origSourceHeight);
	}
	printf("CWindowManager::ILMSurfaceCallbackFunction-out: surface (%d) created\n",id);
}
//-----------------------------------------------------------------------------
void CWindowManager::ILMCallBackFunction(ilmObjectType object, t_ilm_uint id, t_ilm_bool created)
{
	struct ilmSurfaceProperties sp;

	if ( object == ILM_SURFACE )
	{
		if (created == ILM_TRUE )
		{
			printf("CWindowManager::ILMCallBackFunction: surface (%d) created\n",id);
			ilm_getPropertiesOfSurface(id, &sp);
			
			if ((sp.origSourceWidth != 0) && (sp.origSourceHeight !=0))
			{   // surface is already configured
				printf("CWindowManager::ILMCallBackFunction: surface (%d) is already configured \n",id);
				this->HandlingSurface(id, sp.origSourceWidth, sp.origSourceHeight);
			}
			else
			{
				// wait for configured event
				printf("CWindowManager::ILMCallBackFunction: surface (%d) wait for configured event \n",id);
				ilm_surfaceAddNotification(id,&ILMSurfaceCallbackFunctionStatic);
				ilm_commitChanges();
			}
		}
		else
		{
			printf("CWindowManager::ILMCallBackFunction: surface (%u) destroyed\n",id);
		}
	}
	else if (object == ILM_LAYER)
	{
		if (created == ILM_TRUE )
			printf("CWindowManager::ILMCallBackFunction: layer (%u) created\n",id);
		else
			printf("CWindowManager::ILMCallBackFunction: layer (%u) destroyed\n",id);
	}
}
//-----------------------------------------------------------------------------
bool CWindowManager::HandlingSurface(t_ilm_uint id, t_ilm_uint width, t_ilm_uint height)
{
	t_ilm_uint lid = 0;
	CIVISurface *psurface = new CIVISurface();
	CIVILayer *player = NULL;


	if (id < (2*1024))
	{
		player = this->m_Screen[0]->GetLayerById(1024);
	}
	else
	{
		player = this->m_Screen[1]->GetLayerById(1024*2);
	}
	psurface->ConfiguredSurface(id);
	psurface->ConfiguredSurface(width, height);

	ilm_surfaceSetDestinationRectangle(id, 0, 0, width, height);
	ilm_surfaceSetSourceRectangle(id, 0, 0, width, height);
	ilm_surfaceSetVisibility(id, ILM_TRUE);
	
	player->AddSurface(psurface);
	
/*
	if (id < (2*1024))
	{
		ilm_layerAddSurface(1*1024,id);
	}
	else
	{
		ilm_layerAddSurface(2*1024,id);
	}
*/	
	ilm_commitChanges();

    printf("layer-add-surfaces: surface (%u) configured with:\n"
           "    dst region: x:0 y:0 w:%u h:%u\n"
           "    src region: x:0 y:0 w:%u h:%u\n"
           "    visibility: TRUE\n"
           "    added to layer\n", id, width, height, width, height);
}
//-----------------------------------------------------------------------------
bool CWindowManager::WindowManagerInitialize()
{
	if (ilm_init() == ILM_FAILED)
	{
		return false;
	}
	
	this->m_Config = new (std::nothrow)CILMConfig();
	
	this->ScreenSearch();
	
	this->CreateLayers();
	
	::ilm_registerNotification(ILMCallBackFunctionStatic, (void*)this);
	
	return true;
}
//-----------------------------------------------------------------------------
bool CWindowManager::CreateLayers()
{
	int layernum;
	
	layernum = this->m_Config->GetLayerNum();
	
	for(int i=0;i < layernum;i++)
	{
		std::string layername, screenname;
		
		if (this->m_Config->GetLayerName(i, layername) == true )
		{
			if (this->m_Config->GetLayerAttachScreen(i, screenname) == true )
			{
				t_ilm_uint id, width, height, z;
				CIVIScreen* psc = this->GetScreenByName(screenname);
				
				if (psc != NULL)
				{
					if (this->m_Config->GetLayerInfo(i, id, width, height, z) == true)
					{
						CIVILayer * ivilayer = new (std::nothrow) CIVILayer();
						ivilayer->CreateLayer(0, 0, 0, width, height, id, layername);
						psc->AddLayer(ivilayer);
					}
				}
			}
		}
	}
	
	return true;
}
//-----------------------------------------------------------------------------
CIVIScreen* CWindowManager::GetScreenByName(std::string name)
{
	int num = m_Screen.size();
	for(int i=0;i < num;i++)
	{
		if (this->m_Screen[i]->GetScreenName() == name)
		{
			return this->m_Screen[i];
		}
	}
	
	return NULL;
}
//-----------------------------------------------------------------------------
bool CWindowManager::ScreenSearch()
{
	struct ilmScreenProperties screenProperties;
	t_ilm_uint* screen_IDs = NULL;
	t_ilm_uint screen_ID = 0;
	t_ilm_uint screen_count = 0;
	t_ilm_uint choosen_width = 0;
	t_ilm_uint choosen_height = 0;
	int i;

	ilm_getScreenIDs(&screen_count, &screen_IDs);

	for (int i = 0; i < screen_count; i++)
	{
		ilm_getPropertiesOfScreen(screen_IDs[i], &screenProperties);
		
		CIVIScreen *psc = new(std::nothrow) CIVIScreen();
		if ( psc != NULL)
		{
			std::string screenname, connectorname = std::string(screenProperties.connectorName);
			if (this->m_Config->GetScreenNameByConnectorName(connectorname, screenname) == true)
			{
				psc->SetParameter(screen_IDs[i], screenname, connectorname, screenProperties.screenWidth, screenProperties.screenHeight);
				int num = this->m_Screen.size();
				this->m_Screen.resize(num+1);
				this->m_Screen[num] = psc;
			}
		}
	}
	
	::free(screen_IDs);
	
	return true;
}
//-----------------------------------------------------------------------------


