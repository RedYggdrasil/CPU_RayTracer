#include "App/Maths/RMath.h"


#define _USE_MATH_DEFINES
#include <math.h>

using namespace AppNmsp;
const double RMath::PI = M_PI;

//Pure math
double RMath::Cos(double rad)
{
	return cos(rad);
}

double RMath::Sin(double rad)
{
	return sin(rad);
}

double RMath::Tan(double rad)
{
	return tan(rad);
}

//DirectXMath
using namespace DirectX;

XMMATRIX RMath::TransformMatrixDeg(const XMFLOAT3& InLocation, const XMFLOAT3& InDegreeRotation, const XMFLOAT3& InScale)
{
	XMVECTOR vTranslation = XMLoadFloat3(&InLocation);
	XMVECTOR vRotation = { XMConvertToRadians(InDegreeRotation.x),XMConvertToRadians(InDegreeRotation.y), XMConvertToRadians(InDegreeRotation.z), 0.f };
	XMVECTOR vScale = XMLoadFloat3(&InScale);

	// Create the transformation matrix
	return XMMatrixTransformation(
		XMVectorZero(), // scaling origin
		XMQuaternionIdentity(), // rotation origin
		vScale, // scale vector
		XMVectorZero(), // rotation origin
		XMQuaternionRotationRollPitchYawFromVector(vRotation), // rotation quaternion
		vTranslation // translation vector
	);
}
XMMATRIX RMath::TransformMatrixRad(const XMFLOAT3& InLocation, const XMFLOAT3& InRadianRotation, const XMFLOAT3& InScale)
{
	XMVECTOR vTranslation = XMLoadFloat3(&InLocation);
	XMVECTOR vRotation = XMLoadFloat3(&InRadianRotation);
	XMVECTOR vScale = XMLoadFloat3(&InScale);

	// Create the transformation matrix
	return XMMatrixTransformation(
		XMVectorZero(), // scaling origin
		XMQuaternionIdentity(), // rotation origin
		vScale, // scale vector
		XMVectorZero(), // rotation origin
		XMQuaternionRotationRollPitchYawFromVector(vRotation), // rotation quaternion
		vTranslation // translation vector
	);
}

DirectX::XMVECTOR RMath::ComputeNormalFromPointsClockwise(DirectX::FXMVECTOR InPointOne, DirectX::FXMVECTOR InPointTwo, DirectX::FXMVECTOR InPointThree)
{
	//Two vectors residing on the plane
	DirectX::XMVECTOR vec1 = DirectX::XMVectorSubtract(InPointTwo, InPointOne);
	DirectX::XMVECTOR vec2 = DirectX::XMVectorSubtract(InPointThree, InPointOne);

	// Cross product gives the normal
	DirectX::XMVECTOR norm = DirectX::XMVector3Cross(vec1, vec2);

	return DirectX::XMVector3Normalize(norm);
}

DirectX::XMFLOAT3 RMath::QuatToEuler(DirectX::XMFLOAT4 InQuaternion)
{
	float w = InQuaternion.w;
	float x = InQuaternion.x;
	float y = InQuaternion.y;
	float z = InQuaternion.z;

	float yaw = atan2f(2.0f * (y * z + w * x), w * w - x * x - y * y + z * z);
	float pitch = asinf(-2.0f * (x * z - w * y));
	float roll = atan2f(2.0f * (x * y + w * z), w * w + x * x - y * y - z * z);
	return { roll , pitch , yaw };
}

DirectX::XMVECTOR XM_CALLCONV RMath::QuatToEulerDegXMVECTOR(DirectX::FXMVECTOR InQuaternion)
{
	float w = DirectX::XMVectorGetW(InQuaternion);
	float x = DirectX::XMVectorGetX(InQuaternion);
	float y = DirectX::XMVectorGetY(InQuaternion);
	float z = DirectX::XMVectorGetZ(InQuaternion);

	float yaw = atan2f(2.0f * (y * z + w * x), w * w - x * x - y * y + z * z);
	float pitch = asinf(-2.0f * (x * z - w * y));
	float roll = atan2f(2.0f * (x * y + w * z), w * w + x * x - y * y - z * z);

	roll = XMConvertToDegrees(roll);
	pitch = XMConvertToDegrees(pitch);
	yaw = XMConvertToDegrees(yaw);

	return XMVectorSet(yaw, pitch, roll, 0.f);
}

DirectX::XMVECTOR XM_CALLCONV RMath::LookAtQuatRad(DirectX::FXMVECTOR InFromPosition, DirectX::FXMVECTOR InToPosition)
{
	return FromForwardVectorQuatRad(InToPosition - InFromPosition);
}

DirectX::XMVECTOR XM_CALLCONV RMath::FromForwardVectorQuatRad(DirectX::FXMVECTOR InForwardVector)
{
	XMVECTOR NewX = XMVector3Normalize(InForwardVector);
	XMFLOAT3 upVec = ((fabsf(XMVectorGetZ(NewX)) < (1.f)) ? XMFLOAT3(0.f, 0.f, 1.f) : XMFLOAT3(1.f, 0.f, 0.f));
	XMVECTOR UpVector = XMLoadFloat3(&upVec);

	XMVECTOR NewY = XMVector3Cross(UpVector, NewX);
	NewY = XMVector3Normalize(NewY);
	XMVECTOR NewZ = XMVector3Cross(NewX, NewY);

	// Create a rotation matrix
	DirectX::XMMATRIX lookAtMatrix = DirectX::XMMatrixIdentity();
	lookAtMatrix.r[0] = NewX; // X axis
	lookAtMatrix.r[1] = NewY; // Y axis
	lookAtMatrix.r[2] = NewZ; // Z axis
	return XMQuaternionRotationMatrix(lookAtMatrix);

}

bool RMath::GetIsDrawClockwise(FXMVECTOR InExpectedNormalDir, DirectX::FXMMATRIX InTransposeMatrix)
{
	XMVECTOR X0Y0 = DirectX::XMVector3Transform({ 0.f, 0.f, 0.f, 1.f }, InTransposeMatrix);
	XMVECTOR X1Y0 = DirectX::XMVector3Transform(XMVECTOR{ 1.f, 0.f, 0.f, 1.f }, InTransposeMatrix);
	XMVECTOR X0Y1 = DirectX::XMVector3Transform(XMVECTOR{ 0.f, 1.f, 0.f, 1.f }, InTransposeMatrix);
	XMVECTOR DeducedClockwiseNormal = RMath::ComputeNormalFromPointsClockwise(X0Y0, X1Y0, X0Y1);
	float vectorX = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DeducedClockwiseNormal, InExpectedNormalDir));

	return vectorX < FLT_EPSILON;
}
