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

//DirectX12
namespace MyEngine {
	class DirectX12
	{
	public:
		static DirectX12* GetInstance();

		//初期化
		void Initialize();

		//DXGIFacyoryの作成
		void DXGIFactory();

		//Adapterの作成
		void Adapter();

		//deviceの作成
		void D3D12Device();

		//Commandの作成
		void Command();

		//Swapchainの作成
		void SwapChain();

		//DescriptorHeapの作成
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

		//ErrorDetection
		void Error();

		//Barrierの作成
		void Barrier();

		//Screenに表示
		void ScreenDisplay();

		//コマンドの確定
		void CommandConfirm();

		//Fenceの作成
		void Fence();

		/*void Update();*/

		//リークチェック
		void ResourceLeakCheck();

		//開放処理
		void Release();

		//ImGuiにDescriptorHeapを設定
		void SetImGuiDescriptorHeap();

		//ImGuiにCommandを送信
		void PushImGuiDrawCommand();

		//RenderTextureResourceの作成
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(DXGI_FORMAT format, const Vector4& clearColor);

		//BufferResourceの作成
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

		//Getter
		Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return device_; }
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return commandList_; }
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue() { return commandQueue_; }

		//DescriptorHeapの作成
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

		//FPS
		void InitializeFixFPS();
		//FPSUpdate
		void UpdataFixFPS();
	public:
		//Getter
		void GetBackBuffer();

		//DSVの作成
		void CreateDSV();
		void CreateDSVParticle();

		//RTVの作成
		void RTV();
		void CreateRTVForRenderTexture();

		//コマンドキック
		void CommandKick();

		//シグナルを待つ
		void Signal();

		//次フレームコマンドリスト
		void NextFlameCommandList();

		//Finalize
		void Finalize();
		//デバッグレイヤー
		void DebugLayer();

		//void DebugLayer();


		//Getter
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetSrvDescriptorHeap() { return srvDescriptorHeap_; }

		D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU() { return textureSrvHandleGPU_; }

		D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc() { return depthStencilDesc_; }
		D3D12_DEPTH_STENCIL_DESC GetDepthStencilDescForParticle() { return depthStencilDescForParticle_; }

		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

		static const uint32_t kMaxSRVCount = 512;
		uint32_t GetdescriptorSizeSRV() { return descriptorSizeSRV; }

		uint64_t GetFenceValue() { return fenceValue_; }
		HANDLE GerFenceEvent() { return fenceEvent_; }
		Microsoft::WRL::ComPtr<ID3D12Fence> GetFence() { return fence_; }
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetCommandAllocator() { return commandAllocator_; }

		//Setter
		void SetFenceValue(int value) { fenceValue_ += value; }
	private:
		static DirectX12* instance;

		WindowsAPI* windowsAPI_ = nullptr;
		Texture* texture_;

		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;

		Microsoft::WRL::ComPtr<ID3D12Device> device_;
		Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;


		DXGI_ADAPTER_DESC3 adapterDesc_;

		//使用するアダプタ用の変数。最初にnullptrを入れておく
		Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter_;

		//コマンドキューを生成する
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc_;

		//コマンドアロケータを生成する
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;

		//コマンドリストを生成する
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

		//スワップチェーンを生成する
		Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc_;

		//ディスクリプターヒープの生成
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_;
		D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc_;

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeapForParticle_;

		//RTVの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_;
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDescForParticle_;

		D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};
		D3D12_DEPTH_STENCIL_DESC depthStencilDescForParticle_{};

		//ディスクリプタの先頭を取得する
		D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle_;

		//RTVを２つ作るのでディスクリプタ２つ用意
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_[2];


		UINT backBufferIndex_;

		Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResource_[2];
		//TransitionBarrierの設定
		D3D12_RESOURCE_BARRIER barrier_;

		//初期値0でFenceを作る
		Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
		uint64_t fenceValue_;

		//FenceのSignalを待つためのイベントを作成する
		HANDLE fenceEvent_;



		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_;

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
}