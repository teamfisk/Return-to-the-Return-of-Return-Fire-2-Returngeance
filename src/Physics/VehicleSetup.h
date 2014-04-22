#ifndef Physics_Vehicle_h__
#define Physics_Vehicle_h__

//#include "PrecompiledHeader.h"

#include <Common/Base/hkBase.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Config/hkConfigVersion.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Container/String/hkStringBuf.h>

// Vehicle page 425 in documentation
#include <Physics2012/Vehicle/hkpVehicleInstance.h>

#include <Physics2012/Vehicle/AeroDynamics/Default/hkpVehicleDefaultAerodynamics.h>
#include <Physics2012/Vehicle/DriverInput/Default/hkpVehicleDefaultAnalogDriverInput.h>
#include <Physics2012/Vehicle/Brake/Default/hkpVehicleDefaultBrake.h>
#include <Physics2012/Vehicle/Engine/Default/hkpVehicleDefaultEngine.h>
#include <Physics2012/Vehicle/VelocityDamper/Default/hkpVehicleDefaultVelocityDamper.h>
#include <Physics2012/Vehicle/Steering/Default/hkpVehicleDefaultSteering.h>
#include <Physics2012/Vehicle/Suspension/Default/hkpVehicleDefaultSuspension.h>
#include <Physics2012/Vehicle/Transmission/Default/hkpVehicleDefaultTransmission.h>
#include <Physics2012/Vehicle/WheelCollide/RayCast/hkpVehicleRayCastWheelCollide.h>
#include <Physics2012/Vehicle/WheelCollide/RayCast/hkpVehicleRayCastWheelCollide.h>
#include <Physics2012/Collide/Filter/Group/hkpGroupFilter.h>

#include "World.h"
#include "Components/Vehicle.h"
#include "Components/Wheel.h"
#include "Components/Transform.h"

class VehicleSetup
{
public:
	virtual void VehicleSetup::buildVehicle(World *world, const hkpWorld* physicsWorld, hkpVehicleInstance& vehicle, EntityID vehicleEntity, std::vector<EntityID> wheelEntities);

public:
	struct WheelData
	{
		Components::Wheel* WheelComponent;
		Components::Transform* TransformComponent;
	};

	std::vector<WheelData> m_Wheels;

	virtual void setupVehicleData(const hkpWorld* world, hkpVehicleData& data);
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultAnalogDriverInput& driverInput);
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultEngine& engine, Components::Vehicle vehicleComponent);
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultSteering& steering, Components::Vehicle vehicleComponent);
	
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultTransmission& transmission, Components::Vehicle vehicleComponent);
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultBrake& brake, Components::Vehicle vehicleComponent );
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultSuspension& suspension, Components::Vehicle vehicleComponent);
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultAerodynamics& aerodynamics, Components::Vehicle vehicleComponent );
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultVelocityDamper& velocityDamper, Components::Vehicle vehicleComponent);

	virtual void setupWheelCollide(const hkpWorld* world, const hkpVehicleInstance& vehicle, hkpVehicleRayCastWheelCollide& wheelCollide);
};

#endif // Physics_Vehicle_h__
