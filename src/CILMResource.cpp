#include "CILMResource.hpp"

#include <string>
#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------
// CIVISurface
//-----------------------------------------------------------------------------
CIVISurface::CIVISurface() : m_IsConfigured(false)
{
	
}
//-----------------------------------------------------------------------------
CIVISurface::~CIVISurface()
{
	
	
}
//-----------------------------------------------------------------------------
bool CIVISurface::ConfiguredSurface(t_ilm_uint width, t_ilm_uint height)
{
	this->m_Width = width;
	this->m_Height = height;
	
}
//-----------------------------------------------------------------------------
bool CIVISurface::ConfiguredSurface(t_ilm_uint id)
{
	this->m_Id = id;
}
//-----------------------------------------------------------------------------
// CIVILayer
//-----------------------------------------------------------------------------
CIVILayer::CIVILayer() : m_X(0), m_Y(0), m_Z(0), m_Width(0), m_Height(0), m_Id(0)
{
	
	
}
//-----------------------------------------------------------------------------
CIVILayer::~CIVILayer()
{
	
	
	
}
//-----------------------------------------------------------------------------
bool CIVILayer::CreateLayer(t_ilm_uint x, t_ilm_uint y, t_ilm_uint z, t_ilm_uint width, t_ilm_uint height,
							t_ilm_uint id, std::string layername )
{
	this->m_X = x;;
	this->m_Y = y;
	this->m_Z = z;
	this->m_Width = width;
	this->m_Height = height;
	this->m_Id = id;
	this->m_LayerName = layername;
	
	ilm_layerCreateWithDimension(&this->m_Id, this->m_Width, this->m_Height);
	ilm_layerSetVisibility(this->m_Id,ILM_TRUE);
	
	printf("Create Layer(%s) id(%u),x(%d),y(%d),z(%d),w(%d),h(%d)\n",this->m_LayerName.c_str(), this->m_Id,
			this->m_X, this->m_Y, this->m_Z, this->m_Width, this->m_Height);
}
//-----------------------------------------------------------------------------
bool CIVILayer::AddSurface(CIVISurface *psurface)
{
	int num = this->m_Surfaces.size();
	this->m_Surfaces.resize(num+1);
	
	if (num == 0)
	{
		this->m_Surfaces[0] = psurface;
	}
	else
	{
		for(int i=num; i > 0; i--)
		{
			if (this->m_Surfaces[i-1]->GetSurfaceZ() > psurface->GetSurfaceZ())
			{
				this->m_Surfaces[i] = this->m_Surfaces[i-1];
			}
			else
			{
				this->m_Surfaces[i] = psurface;
				break;
			}
		}
	}
	
	num = this->m_Surfaces.size();
	
	t_ilm_layer *porder = (t_ilm_layer *)::malloc(sizeof(t_ilm_layer) * num);
	::memset(porder,0,sizeof(t_ilm_layer) * num);
	
	for(int i=0;i < num;i++)
	{
		porder[i] = this->m_Surfaces[i]->GetSurfaceId();
	}
	
	printf("CIVILayer::AddSurface: surface (%u) adding\n",this->m_Id);
	
	ilm_layerSetRenderOrder(this->m_Id, porder, num);
	
	printf("CIVILayer::AddSurface: surface (%u) added\n",this->m_Id);
	
	::free(porder);
	
	return true;
}
//-----------------------------------------------------------------------------
CIVISurface* CIVILayer::GetSurfaceById(t_ilm_uint id)
{
	int num = this->m_Surfaces.size();

	for(int i=0; i < num; i++)
	{
		if (this->m_Surfaces[i]->GetSurfaceId() == id)
		{
			return this->m_Surfaces[i];
		}
	}
	
	return NULL;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// CIVIScreen
//-----------------------------------------------------------------------------
CIVIScreen::CIVIScreen() : m_Width(0), m_Height(0), m_Id(0)
{
	
	
}
//-----------------------------------------------------------------------------
CIVIScreen::~CIVIScreen()
{
	
	
}
//-----------------------------------------------------------------------------
void CIVIScreen::SetParameter(t_ilm_uint id, std::string sname, std::string cname, t_ilm_uint width, t_ilm_uint height)
{
	this->m_Name = sname;
	this->m_ConnectorName = cname;
	this->m_Width = width;
	this->m_Height = height;
	this->m_Id = id;
	
	printf("Screen: name=%s (%s)  size(%ux%u) id:%u\n",m_Name.c_str(),m_ConnectorName.c_str(), width, height, id);
}
//-----------------------------------------------------------------------------
bool CIVIScreen::AddLayer(CIVILayer *player)
{
	int num = this->m_Layers.size();
	this->m_Layers.resize(num+1);
	
	if (num == 0)
	{
		m_Layers[0] = player;
	}
	else
	{
		for(int i=num; i > 0; i--)
		{
			if (this->m_Layers[i-1]->GetLayerZ() > player->GetLayerZ())
			{
				this->m_Layers[i] = this->m_Layers[i-1];
			}
			else
			{
				this->m_Layers[i] = player;
				break;
			}
		}
	}
	
	num = this->m_Layers.size();
	
	t_ilm_layer *porder = (t_ilm_layer *)::malloc(sizeof(t_ilm_layer) * num);
	::memset(porder,0,sizeof(t_ilm_layer) * num);
	
	for(int i=0;i < num;i++)
	{
		porder[i] = this->m_Layers[i]->GetLayerId();
	}
	
	ilm_displaySetRenderOrder(this->m_Id, porder, num);

	printf("CIVIScreen::AddLayer: screen %u layer (%u) added\n",this->m_Id, player->GetLayerId() );
	
	::free(porder);
	
	return true;
}
//-----------------------------------------------------------------------------
CIVILayer* CIVIScreen::GetLayerById(t_ilm_uint id)
{
	int num = this->m_Layers.size();

	for(int i=0; i < num; i++)
	{
		if (this->m_Layers[i]->GetLayerId() == id)
		{
			return this->m_Layers[i];
		}
	}
	
	return NULL;
}
//-----------------------------------------------------------------------------
CIVILayer* CIVIScreen::GetLayerByName(std::string layername)
{
	int num = this->m_Layers.size();

	for(int i=0; i < num; i++)
	{
		if (this->m_Layers[i]->GetLayerId() == id)
		{
			return this->m_Layers[i];
		}
	}
	
	return NULL;
}
//-----------------------------------------------------------------------------
CIVISurface* CIVIScreen::GetSurfaceById(t_ilm_uint id)
{
	int num = this->m_Layers.size();

	for(int i=0; i < num; i++)
	{
		CIVILayer* player = this->m_Layers[i];
		
		CIVISurface* psurface = player->GetSurfaceById(id);

		if (psurface != NULL) return psurface;
	}
	
	return NULL;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------


