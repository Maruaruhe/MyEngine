#include "DirectX12.h"
#include "../Log/Log.h"
#include "../../Object/Texture/Texture.h"

#include <thread>

using namespace MyEngine;

DirectX12* DirectX12::instance = nullptr;

DirectX12* DirectX12::GetInstance() {
	//static DirectX12 instance;
	//return &instance;
	if (instance == nullptr) {
		instance = new DirectX12;
	}
	return instance;
}

void DirectX12::Initialize() {
	InitializeFixFPS();
	DebugLayer();

	windowsAPI_ = WindowsAPI::GetInstance();
	windowsAPI_->Init();
	DebugLayer();

	DXGIFactory();
	Adapter();
	D3D12Device();
	descriptorSizeSRV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descriptorSizeRTV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	descriptorSizeDSV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	Error();
	Command();
	SwapChain();
	DescriptorHeap();
	CreateDSV();
	CreateDSVParticle();
	Fence();

	//ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(windowsAPI_->GetHwnd());
	ImGui_ImplDX12_Init(device_.Get(),
		swapChainDesc_.BufferCount,
		rtvDesc_.Format,
		srvDescriptorHeap_.Get(),
		srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart());
}


void DirectX12::PreDraw() {
	GetBackBuffer();
	Barrier();
	RTV();
	SetImGuiDescriptorHeap();
}

void DirectX12::PostDraw() {
	PushImGuiDrawCommand();
	ScreenDisplay();
	CommandConfirm();
	CommandKick();

	UpdataFixFPS();

	Signal();
	NextFlameCommandList();
}

void DirectX12::DXGIFactory() {
	dxgiFactory_ = nullptr;
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(hr));
}

void DirectX12::Adapter() {
	useAdapter_ = nullptr;

		for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) != DXGI_ERROR_NOT_FOUND; ++i) {
		//GetAdapterInfo
		adapterDesc_ = {};
		HRESULT hr = useAdapter_->GetDesc3(&adapterDesc_);
		assert(SUCCEEDED(hr));
		//ソフトウェアでなければok
		if (!(adapterDesc_.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//logに出力
			LogText(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc_.Description)));
			break;
		}
		useAdapter_ = nullptr;
	}
	assert(useAdapter_ != nullptr);
}

void DirectX12::D3D12Device() {
	device_ = nullptr;
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0 };
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		HRESULT hr = D3D12CreateDevice(useAdapter_.Get(), featureLevels[i], IID_PPV_ARGS(&device_));
		if (SUCCEEDED(hr)) {
			LogText(std::format("FeatureLevel:{}\n", featureLevelStrings[i]));
			break;
		}
	}
}

void DirectX12::Command() {
	commandQueue_ = nullptr;
	commandQueueDesc_ = {};
	HRESULT hr = device_->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(hr));

	commandAllocator_ = nullptr;
	hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(hr));

	commandList_ = nullptr;
	hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(hr));
}

void DirectX12::SwapChain() {
	swapChain_ = nullptr;
	swapChainDesc_ = {};

	swapChainDesc_.Width = kClientWidth;
	swapChainDesc_.Height = kClientHeight;
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc_.SampleDesc.Count = 1;
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc_.BufferCount = 2;
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;


	HRESULT hr = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_.Get(), windowsAPI_->GetHwnd(), &swapChainDesc_, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

void DirectX12::DescriptorHeap() {
	//rtv
	rtvDescriptorHeap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	rtvDescriptorHeapDesc_ = {};

	rtvDescriptorHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescriptorHeapDesc_.NumDescriptors = 2;

	HRESULT hr = device_->CreateDescriptorHeap(&rtvDescriptorHeapDesc_, IID_PPV_ARGS(&rtvDescriptorHeap_));
	assert(SUCCEEDED(hr));

	swapChainResource_[0] = { nullptr };
	swapChainResource_[1] = { nullptr };
	hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResource_[0]));
	assert(SUCCEEDED(hr));
	hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResource_[1]));
	assert(SUCCEEDED(hr));

	rtvDesc_ = {};
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	rtvStartHandle_ = rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();

	rtvHandle_[0] = rtvStartHandle_;
	device_->CreateRenderTargetView(swapChainResource_[0].Get(), &rtvDesc_, rtvHandle_[0]);
	rtvHandle_[1].ptr = rtvHandle_[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	device_->CreateRenderTargetView(swapChainResource_[1].Get(), &rtvDesc_, rtvHandle_[1]);
	//srv
	srvDescriptorHeap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount, true);
}

void DirectX12::CreateDSV() {
	//dsv
	dsvDescriptorHeap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);

	dsvDesc_ = {};
	dsvDesc_.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc_.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = texture_->CreateDepthStencilTextureResource(device_, kClientWidth, kClientHeight);
	device_->CreateDepthStencilView(depthStencilResource.Get(), &dsvDesc_, dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());

	depthStencilDesc_.DepthEnable = true;
	depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}
void DirectX12::CreateDSVParticle() {
	//dsv
	dsvDescriptorHeapForParticle_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);

	dsvDescForParticle_ = {};
	dsvDescForParticle_.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDescForParticle_.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResourceForParticle = texture_->CreateDepthStencilTextureResource(device_, kClientWidth, kClientHeight);
	device_->CreateDepthStencilView(depthStencilResourceForParticle.Get(), &dsvDescForParticle_, dsvDescriptorHeapForParticle_->GetCPUDescriptorHandleForHeapStart());

	depthStencilDescForParticle_.DepthEnable = true;
	//epthStencilDescForParticle.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDescForParticle_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDescForParticle_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

void DirectX12::GetBackBuffer() {
	//これから書き込むバックバッファのインデックスを取得
	backBufferIndex_ = swapChain_->GetCurrentBackBufferIndex();
}

void DirectX12::RTV() {
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	commandList_->OMSetRenderTargets(1, &rtvHandle_[backBufferIndex_], false, &dsvHandle);

	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };
	commandList_->ClearRenderTargetView(rtvHandle_[backBufferIndex_], clearColor, 0, nullptr);
}

void DirectX12::CommandKick() {
	ID3D12CommandList* commandLists[] = { commandList_.Get()};
	commandQueue_->ExecuteCommandLists(1, commandLists);

	swapChain_->Present(1, 0);
}

void DirectX12::NextFlameCommandList() {
	HRESULT hr = commandAllocator_->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

void DirectX12::DebugLayer() {
	#ifdef _DEBUG
	ID3D12Debug1* debugControllar = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugControllar)))) {
		//デバッグレイヤーを有効化する
		debugControllar->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugControllar->SetEnableGPUBasedValidation(TRUE);
	}
#endif // _DEBUG
}

void DirectX12::Error() {
#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		//infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		//抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] = {
			//windows11でのDXGいデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			//https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
		//解放
		infoQueue->Release();
	}
#endif // _DEBUG
}

void DirectX12::Barrier() {
	barrier_ = {};
	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier_.Transition.pResource = swapChainResource_[backBufferIndex_].Get();
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	commandList_->ResourceBarrier(1, &barrier_);
}

void DirectX12::ScreenDisplay() {
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commandList_->ResourceBarrier(1, &barrier_);
}

void DirectX12::CommandConfirm() {
	HRESULT hr = commandList_->Close();
	assert(SUCCEEDED(hr));
}

void DirectX12::Fence() {
	fence_ = nullptr;
	fenceValue_ = 0;
	HRESULT hr = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));

	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent_ != nullptr);
}

void DirectX12::Signal() {
	fenceValue_++;
	commandQueue_->Signal(fence_.Get(), fenceValue_);
	if (fence_->GetCompletedValue() < fenceValue_) {
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		WaitForSingleObject(fenceEvent_, INFINITE);
	}
}


Microsoft::WRL::ComPtr<ID3D12Resource> DirectX12::CreateRenderTextureResource(DXGI_FORMAT format, const Vector4& clearColor) {
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

	D3D12_RESOURCE_DESC resourceDesc;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;
	
	device_->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		&clearValue,
		IID_PPV_ARGS(&resource));

	return resource;
}

void DirectX12::CreateRTVForRenderTexture() {
	const Vector4 kRenderTargetClearView{ 1.0f,1.0f,0.0f,1.0f };
	auto renderTextureResource = CreateRenderTextureResource(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearView);

	device_->CreateRenderTargetView(renderTextureResource.Get(), &rtvDesc_, GetCPUDescriptorHandle(2));

	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	renderTextureSrvDesc.Texture2D.MipLevels = 1;

	device_->CreateShaderResourceView(renderTextureResource.Get(), &renderTextureSrvDesc, GetCPUDescriptorHandle(2));
}


void DirectX12::ResourceLeakCheck() {
	IDXGIDebug1* debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}
}

void DirectX12::Release() {
	CloseHandle(fenceEvent_);

	CloseWindow(windowsAPI_->GetHwnd());
}

Microsoft::WRL::ComPtr<ID3D12Resource> DirectX12::CreateBufferResource(size_t sizeInBytes) {

	HRESULT hr;
	//VertexResourceを生成する--------------------------------------------------------------------------------
	//頂点リソース用のヒープの作成の設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties = {};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC ResourceDesc = {};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeInBytes;
	//バッファの場合はこれらは１にする決まり
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際に頂点リソースを作る
	ID3D12Resource* Resource = nullptr;
	hr = device_->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));

	return Resource;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DirectX12::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible) {
	ID3D12DescriptorHeap* descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = device_->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));
	return descriptorHeap;
}

//ImGui
void DirectX12::SetImGuiDescriptorHeap() {
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap_.Get() };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps);
}

void DirectX12::PushImGuiDrawCommand() {
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList_.Get());
}

D3D12_CPU_DESCRIPTOR_HANDLE DirectX12::GetCPUDescriptorHandle(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSizeSRV * index);
	return handleCPU;
}
D3D12_GPU_DESCRIPTOR_HANDLE DirectX12::GetGPUDescriptorHandle(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSizeSRV * index);
	return handleGPU;
}

void DirectX12::InitializeFixFPS() {
	reference_ = std::chrono::steady_clock::now();
}
void DirectX12::UpdataFixFPS() {

	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	if (elapsed < kMinCheckTime) {
		while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	reference_ = std::chrono::steady_clock::now();
}

void DirectX12::Finalize() {
	delete instance;
	instance = nullptr;
}