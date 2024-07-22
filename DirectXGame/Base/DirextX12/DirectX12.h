#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <format>
#include <dxgidebug.h>
#include "../../Math/Vector4.h"
#include "../../Base/ImGui/ImGuiWND.h"
#include "../WindowsAPI/WindowsAPI.h"
#include <wrl.h>
#include <chrono>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")

class WindowsAPI;

class Texture;

class DirectX12
{
public:
	static DirectX12* GetInstance();

	void Initialize();

	void DXGIFactory();

	void Adapter();

	void D3D12Device();

	void Command();

	void SwapChain();

	void DescriptorHeap();

	/*void RTV();*/

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	void Error();

	void Barrier();

	void ScreenDisplay();

	void CommandConfirm();

	void Fence();

	/*void Update();*/

	void ResourceLeakCheck();

	void Release();

	void SetImGuiDescriptorHeap();

	void PushImGuiDrawCommand();

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(DXGI_FORMAT format,const Vector4& clearColor);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);


	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return device; }

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return commandList; }

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	//FPS
	void InitializeFixFPS();
	void UpdataFixFPS();
public:
	void GetBackBuffer();

	void CreateDSV();
	void CreateDSVParticle();

	void RTV();
	void CreateRTVForRenderTexture();

	void CommandKick();

	void Signal();

	void NextFlameCommandList();

	void Finalize();

	void DebugLayer();

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetSrvDescriptorHeap() { return srvDescriptorHeap; }


	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU() { return textureSrvHandleGPU; }

	D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc() { return depthStencilDesc; }
	D3D12_DEPTH_STENCIL_DESC GetDepthStencilDescForParticle() { return depthStencilDescForParticle; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	static const uint32_t kMaxSRVCount = 512;
	uint32_t GetdescriptorSizeSRV() { return descriptorSizeSRV; }
private:
	static DirectX12* instance;

	WindowsAPI* windowsAPI_ = nullptr;
	Texture* texture;

	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;


	DXGI_ADAPTER_DESC3 adapterDesc;

	//使用するアダプタ用の変数。最初にnullptrを入れておく
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter;

	//コマンドキューを生成する
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;

	//コマンドアロケータを生成する
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;

	//コマンドリストを生成する
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;

	//スワップチェーンを生成する
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;

	//ディスクリプターヒープの生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeapForParticle;

	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDescForParticle;

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	D3D12_DEPTH_STENCIL_DESC depthStencilDescForParticle{};

	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle;

	//RTVを２つ作るのでディスクリプタ２つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle[2];


	UINT backBufferIndex;

	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResource[2];
	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier;

	//初期値0でFenceを作る
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	uint64_t fenceValue;

	//FenceのSignalを待つためのイベントを作成する
	HANDLE fenceEvent;



	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap;

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	uint32_t descriptorSizeSRV;
	uint32_t descriptorSizeRTV;
	uint32_t descriptorSizeDSV;

	std::chrono::steady_clock::time_point reference_;

	DirectX12() = default;
	~DirectX12() = default;
	DirectX12(DirectX12&) = delete;
	DirectX12& operator=(DirectX12&) = delete;
};

