#include "CWindowManager.hpp"

#include <string>
#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <unistd.h>


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
	if ((unsigned)m & ILM_NOTIFICATION_CONFIGURED)
	{
#ifdef _USER_DEBUG_
		printf("CWindowManager::ILMSurfaceCallbackFunction: surface (%d) configured \n",id);
#endif
		this->HandlingSurface(id, sp->origSourceWidth, sp->origSourceHeight);
	}
}
//-----------------------------------------------------------------------------
void CWindowManager::ILMCallBackFunction(ilmObjectType object, t_ilm_uint id, t_ilm_bool created)
{
	struct ilmSurfaceProperties sp;

	if ( object == ILM_SURFACE )
	{
		if (created == ILM_TRUE )
		{
			ilm_getPropertiesOfSurface(id, &sp);
			
			if ((sp.origSourceWidth != 0) && (sp.origSourceHeight !=0))
			{   // surface is already configured
#ifdef _USER_DEBUG_
				printf("CWindowManager::ILMCallBackFunction: surface (%d) is already configured \n",id);
#endif
				this->HandlingSurface(id, sp.origSourceWidth, sp.origSourceHeight);
			}
			else
			{
				// wait for configured event
#ifdef _USER_DEBUG_
				printf("CWindowManager::ILMCallBackFunction: surface (%d) wait for configured event \n",id);
#endif
				ilm_surfaceAddNotification(id,&ILMSurfaceCallbackFunctionStatic);
				ilm_commitChanges();
			}
		}
		else
		{
#ifdef _USER_DEBUG_
			printf("CWindowManager::ILMCallBackFunction: surface (%d) remove \n",id);
#endif
			ilm_surfaceRemoveNotification(id);
			this->RemoveSurface(id);
		}
	}
	else if (object == ILM_LAYER)
	{
#ifdef _USER_DEBUG_
		if (created == ILM_TRUE )
			printf("CWindowManager::ILMCallBackFunction: layer (%u) created\n",id);
		else
			printf("CWindowManager::ILMCallBackFunction: layer (%u) destroyed\n",id);
#endif
	}
}
//-----------------------------------------------------------------------------
bool CWindowManager::HandlingSurface(t_ilm_uint id, t_ilm_uint width, t_ilm_uint height)
{
	t_ilm_uint lid = 0;
	t_ilm_uint x=0, y=0, z=0;
	std::string surfacename, layername;
	
	if (this->m_Config->GetSurfaceInfoById(id, surfacename, layername, x, y, z) == true)
	{
		CIVISurface *psurface = new CIVISurface();
		CIVILayer *player = NULL;
		
		player = this->GetLayerByName(layername);
		
		if (player != NULL)
		{
			psurface->ConfiguredSurface(id, x, y, z, width, height);
			
			player->AddSurface(psurface);
			
			ilm_commitChanges();
		}
	}
#ifdef _USER_DEBUG_
	printf("layer-add-surfaces: surface (%u) configured with:\n"
           "    dst region: x:0 y:0 w:%u h:%u\n"
           "    src region: x:0 y:0 w:%u h:%u\n"
           "    visibility: TRUE\n"
           "    added to layer\n", id, width, height, width, height);
#endif
}
//-----------------------------------------------------------------------------
bool CWindowManager::RemoveSurface(t_ilm_uint id)
{
	CIVISurface* psurface = this->GetSurfaceById(id);
	
	if (psurface != NULL)
	{
		CIVILayer *player = psurface->GetParentLayer();
		player->RemoveSurface(psurface);
		ilm_commitChanges();
		
		delete psurface;
		return true;
	}
	
	return false;
}
//-----------------------------------------------------------------------------
bool CWindowManager::WindowManagerInitialize()
{
	while (ilm_init() == ILM_FAILED)
	{
		usleep(10000);	//10ms
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
CIVILayer* CWindowManager::GetLayerByName(std::string layername)
{
	int num = m_Screen.size();
	for(int i=0;i < num;i++)
	{
		CIVILayer* pLayer = this->m_Screen[i]->GetLayerByName(layername);
		if (pLayer != NULL)
		{
			return pLayer;
		}
	}
	
	return NULL;
}
//-----------------------------------------------------------------------------
CIVILayer* CWindowManager::GetLayerById(t_ilm_uint id)
{
	int num = m_Screen.size();
	for(int i=0;i < num;i++)
	{
		CIVILayer* pLayer = this->m_Screen[i]->GetLayerById(id);
		if (pLayer != NULL)
		{
			return pLayer;
		}
	}
	
	return NULL;
}
//-----------------------------------------------------------------------------
CIVISurface* CWindowManager::GetSurfaceById(t_ilm_uint id)
{
	int num = m_Screen.size();
	for(int i=0;i < num;i++)
	{
		CIVISurface* psurface = this->m_Screen[i]->GetSurfaceById(id);
		if (psurface != NULL)
		{
			return psurface;
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


