#include "Aircraft.hpp"
#include "Game.hpp"

Aircraft::Aircraft(Type type, Game* game) : Entity(game)
	, mType(type)
{
	switch (type)
	{
	case (Eagle):
		mSprite = "Eagle";
		break;
	case (Raptor):
		mSprite = "Raptor";
		break;
	default:
		mSprite = "Eagle";
		break;
	}
}

void Aircraft::updateCurrent(const GameTimer& gt)
{
	// call base class update function
	Entity::updateCurrent(gt);

	//if aircraft moves beyond left boundary
	if (getWorldPosition().x < -1.0f) {
		// make the aircraft move right
		setVelocity(1.0f, 0.0f, 0.0f);
	}

	// if aircraft moves beyond right boundary
	if (getWorldPosition().x > 1.0f) {
		// make aircraft move left
		setVelocity(-1.0f, 0.0f, 0.0f);
	}
}

void Aircraft::drawCurrent(ID3D12GraphicsCommandList* cmdList, FrameResource* mCurrFrameResource) const
{	
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	// get pointers to constant buffers
	auto objectCB = mCurrFrameResource->ObjectCB->Resource();
	auto matCB = mCurrFrameResource->MaterialCB->Resource();

	//set the buffers and topology
	cmdList->IASetVertexBuffers(0, 1, &renderer->Geo->VertexBufferView());
	cmdList->IASetIndexBuffer(&renderer->Geo->IndexBufferView());
	cmdList->IASetPrimitiveTopology(renderer->PrimitiveType);

	
	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(game->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
	tex.Offset(renderer->Mat->DiffuseSrvHeapIndex, game->GetDescriptorHeapSize());

	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + renderer->ObjCBIndex * objCBByteSize;
	D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + renderer->Mat->MatCBIndex * matCBByteSize;

	//set root signature parameters for rendering
	cmdList->SetGraphicsRootDescriptorTable(0, tex);
	cmdList->SetGraphicsRootConstantBufferView(1, objCBAddress);
	cmdList->SetGraphicsRootConstantBufferView(3, matCBAddress);

	cmdList->DrawIndexedInstanced(renderer->IndexCount, 1, renderer->StartIndexLocation, renderer->BaseVertexLocation, 0);
}

void Aircraft::buildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	renderer = render.get();
	renderer->World = getTransform();
	renderer->ObjCBIndex = game->getRenderItems().size();
	renderer->Mat = game->getMaterials()[mSprite].get();
	renderer->Geo = game->getGeometries()["boxGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderer->IndexCount = renderer->Geo->DrawArgs["box"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["box"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["box"].BaseVertexLocation;

	game->getRenderItems().push_back(std::move(render));

}
