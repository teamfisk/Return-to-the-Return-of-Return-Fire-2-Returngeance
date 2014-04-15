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
#include <Physics2012/Vehicle/TyreMarks/hkpTyremarksInfo.h>

#include <Physics2012/Vehicle/WheelCollide/RayCast/hkpVehicleRayCastWheelCollide.h>

#include <Physics2012/Collide/Filter/Group/hkpGroupFilter.h>

class VehicleSetup
{
public:
	virtual void buildVehicle(const hkpWorld* world, hkpVehicleInstance& vehicle);

public:

	virtual void setupVehicleData(const hkpWorld* world, hkpVehicleData& data);
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultAnalogDriverInput& driverInput);
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultSteering& steering);
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultEngine& engine);
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultTransmission& transmission);
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultBrake& brake);
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultSuspension& suspension);
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultAerodynamics& aerodynamics);
	virtual void setupComponent(const hkpVehicleData& data, hkpVehicleDefaultVelocityDamper& velocityDamper);

	virtual void setupWheelCollide(const hkpWorld* world, const hkpVehicleInstance& vehicle, hkpVehicleRayCastWheelCollide& wheelCollide);
	virtual void setupTyremarks(const hkpVehicleData& data, hkpTyremarksInfo& tyremarkscontroller);
};

#endif // Physics_Vehicle_h__
