#ifndef	CWINDOW_MANAGER_H
#define	CWINDOW_MANAGER_H
//-----------------------------------------------------------------------------
#include <vector>
#include <string>

#include <ilm_control.h>

#include "CILMResource.hpp"
#include "CILMConfig.hpp"

//-----------------------------------------------------------------------------
class CWindowManager
{
private:
protected:
	CILMConfig *m_Config;
	std::vector< CIVIScreen* > m_Screen;
	
	static void ILMCallBackFunctionStatic(ilmObjectType object, t_ilm_uint id, t_ilm_bool created, void *user_data);
	static void ILMSurfaceCallbackFunctionStatic(t_ilm_uint id, struct ilmSurfaceProperties* sp, t_ilm_notification_mask m);
	
	bool ScreenSearch();
	bool CreateLayers();
	
	bool HandlingSurface(t_ilm_uint id, t_ilm_uint width, t_ilm_uint height);
	bool RemoveSurface(t_ilm_uint id);
	
	void ILMCallBackFunction(ilmObjectType object, t_ilm_uint id, t_ilm_bool created);
	void ILMSurfaceCallbackFunction(t_ilm_uint id, struct ilmSurfaceProperties* sp, t_ilm_notification_mask m);
	
	CIVIScreen* GetScreenByName(std::string name);
	CIVILayer* GetLayerByName(std::string layername);
	
	CIVILayer* GetLayerById(t_ilm_uint id);
	CIVISurface* GetSurfaceById(t_ilm_uint id);
	
	// singleton
	static CWindowManager m_Win;
	
	CWindowManager();
	CWindowManager(const CWindowManager&) = delete;
	CWindowManager &operator = (const CWindowManager&) = delete;
	
	~CWindowManager();
	
public:
	// set method
	
	
	// get method
	
	
	// methods
	bool WindowManagerInitialize();
	
	
	// singleton
	static CWindowManager* getInstance();
};
//-----------------------------------------------------------------------------
#endif	//#ifndef	CWINDOW_MANAGER_H
