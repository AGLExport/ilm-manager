#ifndef	CILMResource_H
#define	CILMResource_H
//-----------------------------------------------------------------------------
#include <vector>
#include <string>

#include "ilm_control.h"

//-----------------------------------------------------------------------------
class CIVISurface
{
private:
protected:
	bool m_IsConfigured;
	
	t_ilm_uint m_X;
	t_ilm_uint m_Y;
	t_ilm_uint m_Z;
	t_ilm_uint m_Width;
	t_ilm_uint m_Height;
	t_ilm_uint m_Id;
	
public:
	// set method
	
	
	// get method
	t_ilm_uint GetSurfaceId() { return this->m_Id; }
	t_ilm_uint GetSurfaceWidth(){ return this->m_Width; }
	t_ilm_uint GetSurfaceHight(){ return this->m_Height; }
	t_ilm_uint GetSurfaceX(){ return this->m_X; }
	t_ilm_uint GetSurfaceY(){ return this->m_Y; }
	t_ilm_uint GetSurfaceZ(){ return this->m_Z; }
	
	// method
	bool ConfiguredSurface(t_ilm_uint width, t_ilm_uint height);
	bool ConfiguredSurface(t_ilm_uint id);
	
	CIVISurface();
	CIVISurface(const CIVISurface&) = delete;
	CIVISurface &operator = (const CIVISurface&) = delete;
	
	virtual ~CIVISurface();
};
//-----------------------------------------------------------------------------
class CIVILayer
{
private:
protected:
	std::vector< CIVISurface* > m_Surfaces;
	
	t_ilm_uint m_X;
	t_ilm_uint m_Y;
	t_ilm_uint m_Z;
	t_ilm_uint m_Width;
	t_ilm_uint m_Height;
	t_ilm_uint m_Id;
	
public:
	// set method
	bool AddSurface(CIVISurface *psurface);
	
	// get method
	t_ilm_uint GetLayerId() { return this->m_Id; }
	t_ilm_uint GetLayerWidth(){ return this->m_Width; }
	t_ilm_uint GetLayerHight(){ return this->m_Height; }
	t_ilm_uint GetLayerX(){ return this->m_X; }
	t_ilm_uint GetLayerY(){ return this->m_Y; }
	t_ilm_uint GetLayerZ(){ return this->m_Z; }
	
	CIVISurface* GetSurfaceById(t_ilm_uint id);
	
	//method
	bool CreateLayer(t_ilm_uint x, t_ilm_uint y, t_ilm_uint z, t_ilm_uint width, t_ilm_uint height, t_ilm_uint id);
	
	
	CIVILayer();
	CIVILayer(const CIVILayer&) = delete;
	CIVILayer &operator = (const CIVILayer&) = delete;
	
	virtual ~CIVILayer();
};
//-----------------------------------------------------------------------------
class CIVIScreen
{
private:
protected:
	std::vector< CIVILayer* > m_Layers;
	
	std::string m_ConnectorName;
	t_ilm_uint m_Width;
	t_ilm_uint m_Height;
	t_ilm_uint m_Id;
	
public:
	// set method
	bool AddLayer(CIVILayer *player);
	
	// get method
	t_ilm_uint GetScreenId() { return this->m_Id; }
	t_ilm_uint GetScreenWidth(){ return this->m_Width; }
	t_ilm_uint GetScreenHight(){ return this->m_Height; }
	
	CIVILayer* GetLayerById(t_ilm_uint id);
	CIVISurface* GetSurfaceById(t_ilm_uint id);
	
	//method
	void SetParameter(t_ilm_uint id, std::string name, t_ilm_uint width, t_ilm_uint height);
	
	CIVIScreen();
	CIVIScreen(const CIVIScreen&) = delete;
	CIVIScreen &operator = (const CIVIScreen&) = delete;
	
	virtual ~CIVIScreen();
};
//-----------------------------------------------------------------------------
#endif	//#ifndef	CILMResource_H
