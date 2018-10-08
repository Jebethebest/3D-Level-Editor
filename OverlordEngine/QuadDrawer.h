#pragma once
class QuadDrawer
{
public:
	QuadDrawer();
	~QuadDrawer();

	void Initialize(ID3D11Device* pDevice);
	void DrawImmediate(ID3D11DeviceContext* pDeviceContext, Material* pMaterial);
	void DrawImmediate(ID3D11DeviceContext* pDeviceContext, Material* pMaterial, int x, int y, int width, int height);

private:

	bool m_IsInitialized = false;
	VertexPosTex m_vertexArray[4];
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	float m_ScreenWidth = 0.0f, m_ScreenHeight = 0.0f;
	float m_HalfScreenWidth = 0.0f, m_HalfScreenHeight = 0.0f;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	QuadDrawer(const QuadDrawer &obj);
	QuadDrawer& operator=(const QuadDrawer& obj);
};

