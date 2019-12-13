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
void CWindowManager::ILMSurfaceCallbackFunctionStatic(t_ilm_uint id, struct ilmSurfaceProperties* sp, t_ilm_notification_mask m)
{
	extern CWindowManager *g_pwin;
	
	printf("CWindowManager::ILMSurfaceCallbackFunctionStatic: surface (%d) created\n",id);
	g_pwin->ILMSurfaceCallbackFunction(id, sp, m);
}
//-----------------------------------------------------------------------------
void CWindowManager::ILMCallBackFunctionStatic(ilmObjectType object, t_ilm_uint id, t_ilm_bool created, void *user_data)
{
	CWindowManager *p = (CWindowManager*)user_data;
	
	printf("CWindowManager::ILMCallBackFunctionStatic: surface (%d) created\n",id);
	
	p->ILMCallBackFunction(object, id, created);
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
	
	this->ScreenSearch();
	
	int num = m_Screen.size();
	for(int i=0;i < num;i++)
	{
		CIVILayer * ivilayer = new (std::nothrow) CIVILayer();
		ivilayer->CreateLayer(0, 0, 0, m_Screen[i]->GetScreenWidth(), m_Screen[i]->GetScreenHight(), ((i+1)*1024));
		this->m_Screen[i]->AddLayer(ivilayer);
	}
	
	::ilm_registerNotification(ILMCallBackFunctionStatic, (void*)this);
	
	return true;
}
//-----------------------------------------------------------------------------
int CWindowManager::ScreenSearch()
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
			psc->SetParameter(screen_IDs[i], std::string(screenProperties.connectorName), screenProperties.screenWidth, screenProperties.screenHeight);
			int num = this->m_Screen.size();
			this->m_Screen.resize(num+1);
			this->m_Screen[num] = psc;
		}
	}
	
	::free(screen_IDs);
}
//-----------------------------------------------------------------------------


