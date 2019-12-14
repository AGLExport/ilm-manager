#include "CILMConfig.hpp"

#include <string>
#include <iostream>
#include <fstream>

#include <stdlib.h>

const char DEFAULT_CONF_PATH[] = u8"/etc/";

//-----------------------------------------------------------------------------
CILMConfig::CILMConfig()
{
	std::string confpath;
	char *str = getenv("ILM_MANAGER_CONF");
	
	if (str != NULL)
	{
		confpath = std::string(str);
	}
	
	if (confpath.empty()) confpath = std::string(DEFAULT_CONF_PATH);
	
	std::string filename = confpath + std::string("agl.json");
	std::ifstream jsonfile(filename);
	
	if (jsonfile.is_open())
	{
		char buf[256];
		std::string jsonstring;
		size_t size;
		
		do
		{
			jsonfile.getline(buf,256);
			jsonstring = jsonstring + std::string(buf);
		}
		while(jsonfile.eof() == false);
		
		m_JsonString = jsonstring;	//for debug
		
		Json::Reader reader;
		if (reader.parse(jsonstring, this->m_JsonValue) == false)
		{
			printf("fail to json parse :%s\n%s\n",filename.c_str(),jsonstring.c_str());
		}
	}
	else
	{
		printf("fail file open :%s\n",filename.c_str());
	}
}
//-----------------------------------------------------------------------------
CILMConfig::~CILMConfig()
{
	
}
//-----------------------------------------------------------------------------
bool CILMConfig::GetScreenNameByConnectorName(std::string connectorname, std::string &screenname)
{
	if (this->m_JsonValue["screen"].isArray() == true )
	{
		//have a child value
		Json::Value screens = this->m_JsonValue["screen"];
		
		Json::Value::ArrayIndex count, max;
		max = screens.size();
		
		for(count = 0; count < max; count++)
		{
			Json::Value screen = screens[count];
			
			// must need parameta
			if ( screen.isMember("name") == true && screen.isMember("dispname") == true )
			{
				std::string connector = screen["dispname"].asString();
			
				if ( connector == connectorname)
				{
					screenname = screen["name"].asString();
					return true;
				}
				else
				{	}
			}
			else { }
		}
	}
	
	return false;
}
//-----------------------------------------------------------------------------
int CILMConfig::GetLayerNum()
{
	int num = 0;
	
	if (this->m_JsonValue["layer"].isArray() == true )
	{
		//have a child value
		Json::Value layers = this->m_JsonValue["layer"];
		
		Json::Value::ArrayIndex count, max;
		max = layers.size();
		num = int(max);
	}
	
	return num;
}
//-----------------------------------------------------------------------------
bool CILMConfig::GetLayerName(int num, std::string &name)
{
	if (this->m_JsonValue["layer"].isArray() == true )
	{
		//have a child value
		Json::Value layers = this->m_JsonValue["layer"];
		
		Json::Value::ArrayIndex max;
		max = layers.size();
		
		if (num < int(max))
		{
			Json::Value layer = layers[Json::Value::ArrayIndex(num)];
			
			// must need parameta
			if ( layer.isMember("name") == true )
			{
				name = layer["name"].asString();
				return true;
			}
		}
	}
	return false;
}
//-----------------------------------------------------------------------------
bool CILMConfig::GetLayerAttachScreen(int num, std::string &name)
{
	if (this->m_JsonValue["layer"].isArray() == true )
	{
		//have a child value
		Json::Value layers = this->m_JsonValue["layer"];
		
		Json::Value::ArrayIndex max;
		max = layers.size();
		
		if (num < int(max))
		{
			Json::Value layer = layers[Json::Value::ArrayIndex(num)];
			
			// must need parameta
			if ( layer.isMember("attach") == true )
			{
				name = layer["attach"].asString();
				return true;
			}
		}
	}
	return false;
}
//-----------------------------------------------------------------------------
bool CILMConfig::GetLayerInfo(int num, t_ilm_uint &id, t_ilm_uint &width, t_ilm_uint &height, t_ilm_uint &z)
{
	if (this->m_JsonValue["layer"].isArray() == true )
	{
		//have a child value
		Json::Value layers = this->m_JsonValue["layer"];
		
		Json::Value::ArrayIndex max;
		max = layers.size();
		
		if (num < int(max))
		{
			Json::Value layer = layers[Json::Value::ArrayIndex(num)];
			
			// must need parameta
			if ( layer.isMember("id") == true && layer.isMember("width") == true 
				&& layer.isMember("height") == true && layer.isMember("z") == true )
			{
				id = t_ilm_uint(layer["id"].asUInt());
				width = t_ilm_uint(layer["width"].asUInt());
				height = t_ilm_uint(layer["height"].asUInt());
				z = t_ilm_uint(layer["z"].asUInt());
				
				return true;
			}
		}
	}
	return false;
}
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------


