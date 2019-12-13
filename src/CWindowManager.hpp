#ifndef	CWINDOW_MANAGER_H
#define	CWINDOW_MANAGER_H
//-----------------------------------------------------------------------------
#include <vector>
#include <string>

#include "ilm_control.h"

#include "CILMResource.hpp"
//-----------------------------------------------------------------------------
class CWindowManager
{
private:
protected:
	std::vector< CIVIScreen* > m_Screen;
	
	static void ILMCallBackFunctionStatic(ilmObjectType object, t_ilm_uint id, t_ilm_bool created, void *user_data);
	static void ILMSurfaceCallbackFunctionStatic(t_ilm_uint id, struct ilmSurfaceProperties* sp, t_ilm_notification_mask m);
	
	int ScreenSearch();
	bool HandlingSurface(t_ilm_uint id, t_ilm_uint width, t_ilm_uint height);
	
	void ILMCallBackFunction(ilmObjectType object, t_ilm_uint id, t_ilm_bool created);
	void ILMSurfaceCallbackFunction(t_ilm_uint id, struct ilmSurfaceProperties* sp, t_ilm_notification_mask m);
	
	
public:
	// set method
	
	
	// get method
	
	
	// methods
	bool WindowManagerInitialize();
	
	CWindowManager();
	CWindowManager(const CWindowManager&) = delete;
	CWindowManager &operator = (const CWindowManager&) = delete;
	
	virtual ~CWindowManager();
};
//-----------------------------------------------------------------------------
#endif	//#ifndef	CWINDOW_MANAGER_H
