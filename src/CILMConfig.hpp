#ifndef	CILM_CONFIG_H
#define	CILM_CONFIG_H
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <json/json.h>
#include <ilm_control.h>

//-----------------------------------------------------------------------------
class CILMConfig
{
private:
protected:
	std::string m_JsonString;
	Json::Value m_JsonValue;
	
public:
	// set method
	
	
	// get method
	// screen
	bool GetScreenNameByConnectorName(std::string connectorname, std::string &screenname);
	
	// layer
	int GetLayerNum();
	bool GetLayerName(int num, std::string &name);
	bool GetLayerAttachScreen(int num, std::string &name);
	bool GetLayerInfo(int num, t_ilm_uint &id, t_ilm_uint &width, t_ilm_uint &height, t_ilm_uint &x, t_ilm_uint &y, t_ilm_uint &z);
	
	// surface
	bool GetSurfaceInfoById(t_ilm_uint id, std::string &surfacename, std::string &layername, t_ilm_uint &x, t_ilm_uint &y, t_ilm_uint &z);
	
	
	CILMConfig();
	CILMConfig(const CILMConfig&) = delete;
	CILMConfig &operator = (const CILMConfig&) = delete;
	
	virtual ~CILMConfig();
};
//-----------------------------------------------------------------------------
#endif	//#ifndef	CILM_CONFIG_H
