#include "PrecompiledHeader.h"
#include "Physics/VehicleSetup.h"





void VehicleSetup::buildVehicle(World *world, const hkpWorld* physicsWorld, hkpVehicleInstance& vehicle, EntityID vehicleEntity, std::vector<EntityID> wheelEntities)
{
	auto vehicleComponent = world->GetComponent<Components::Vehicle>(vehicleEntity, "Vehicle");
	
	WheelData wheelData;
	for (int i = 0; i < wheelEntities.size(); i++)
	{
		wheelData.WheelComponent = world->GetComponent<Components::Wheel>(wheelEntities[i], "Wheel");
		wheelData.TransformComponent = world->GetComponent<Components::Transform>(wheelEntities[i], "Transform");
		m_Wheels.push_back(wheelData);	
	}
	
	//
	// All memory allocations are made here.
	//
	vehicle.m_data = new hkpVehicleData;
	vehicle.m_driverInput = new hkpVehicleDefaultAnalogDriverInput;
	vehicle.m_steering = new hkpVehicleDefaultSteering;
	vehicle.m_engine = new hkpVehicleDefaultEngine;
	vehicle.m_transmission = new hkpVehicleDefaultTransmission;
	vehicle.m_brake = new hkpVehicleDefaultBrake;
	vehicle.m_suspension = new hkpVehicleDefaultSuspension;
	vehicle.m_aerodynamics = new hkpVehicleDefaultAerodynamics;
	vehicle.m_velocityDamper = new hkpVehicleDefaultVelocityDamper;

	// For illustrative purposes we use a custom hkpVehicleRayCastWheelCollide
	// which implements varying 'ground' friction in a very simple way.
	vehicle.m_wheelCollide = new hkpVehicleRayCastWheelCollide;

	setupVehicleData(physicsWorld, *vehicle.m_data);


	setupComponent(*vehicle.m_data, *static_cast<hkpVehicleDefaultAnalogDriverInput*>(vehicle.m_driverInput));
	setupComponent(*vehicle.m_data, *static_cast<hkpVehicleDefaultSteering*>(vehicle.m_steering), *vehicleComponent);
	setupComponent(*vehicle.m_data, *static_cast<hkpVehicleDefaultEngine*>(vehicle.m_engine), *vehicleComponent);
	setupComponent(*vehicle.m_data, *static_cast<hkpVehicleDefaultTransmission*>(vehicle.m_transmission), *vehicleComponent);
	setupComponent(*vehicle.m_data, *static_cast<hkpVehicleDefaultBrake*>(vehicle.m_brake), *vehicleComponent);
	setupComponent(*vehicle.m_data, *static_cast<hkpVehicleDefaultSuspension*>(vehicle.m_suspension), *vehicleComponent);
	setupComponent(*vehicle.m_data, *static_cast<hkpVehicleDefaultAerodynamics*>(vehicle.m_aerodynamics), *vehicleComponent);
	setupComponent(*vehicle.m_data, *static_cast<hkpVehicleDefaultVelocityDamper*>(vehicle.m_velocityDamper), *vehicleComponent);

	setupWheelCollide(physicsWorld, vehicle, *static_cast<hkpVehicleRayCastWheelCollide*>(vehicle.m_wheelCollide));


	//
	// Check that all components are present.
	//
	HK_ASSERT(0x0		, vehicle.m_data);
	HK_ASSERT(0x7708674a, vehicle.m_driverInput);
	HK_ASSERT(0x5a324a2d, vehicle.m_steering);
	HK_ASSERT(0x7bcb2aff, vehicle.m_engine);
	HK_ASSERT(0x29bddb50, vehicle.m_transmission);
	HK_ASSERT(0x2b0323a2, vehicle.m_brake);
	HK_ASSERT(0x7a7ade23, vehicle.m_suspension);
	HK_ASSERT(0x6ec4d0ed, vehicle.m_aerodynamics);
	HK_ASSERT(0x67161206, vehicle.m_wheelCollide);

	//
	// Set up any variables that store cached data.
	//


	// Give driver input default values so that the vehicle (if this input is a default for non
	// player cars) will drive, even if it is in circles!

	// Steering Defaults
	vehicle.m_deviceStatus = new hkpVehicleDriverInputAnalogStatus;
	hkpVehicleDriverInputAnalogStatus* deviceStatus = (hkpVehicleDriverInputAnalogStatus*)vehicle.m_deviceStatus;
	deviceStatus->m_positionY = 0.f;
	deviceStatus->m_positionX = 0.f;
	deviceStatus->m_handbrakeButtonPressed = false;
	deviceStatus->m_reverseButtonPressed = false;

	//
	// Don't forget to call init! (This function is necessary to set up derived data)
	//
	vehicle.init();
}

void VehicleSetup::setupVehicleData(const hkpWorld* world, hkpVehicleData& data )
{
	data.m_gravity = world->getGravity();

	//
	// The vehicleData contains information about the chassis.
	//

	// The coordinates of the chassis system, used for steering the vehicle.
	//										up					forward				right
	data.m_chassisOrientation.setCols(hkVector4(0, 1, 0), hkVector4(0, 0, -1), hkVector4(1, 0, 0));

	data.m_frictionEqualizer = 0.5f;


	// Inertia tensor for each axis is calculated by using : 
	// (1 / chassis_mass) * (torque(axis)Factor / chassisUnitInertia)
	data.m_torqueRollFactor = 0.625f;
	data.m_torquePitchFactor = 0.5f;
	data.m_torqueYawFactor = 0.35f;

	data.m_chassisUnitInertiaYaw = 1.0f;
	data.m_chassisUnitInertiaRoll = 1.0f;
	data.m_chassisUnitInertiaPitch = 1.0f;

	// Adds or removes torque around the yaw axis 
	// based on the current steering angle.  This will 
	// affect steering.
	data.m_extraTorqueFactor = -0.5f;
	data.m_maxVelocityForPositionalFriction = 0.0f;

	//
	// Wheel specifications
	//
	data.m_numWheels = m_Wheels.size();

	data.m_wheelParams.setSize(data.m_numWheels);

	for (int i = 0; i < m_Wheels.size(); i++)
	{
		data.m_wheelParams[i].m_axle = m_Wheels[i].WheelComponent->AxleID;
		data.m_wheelParams[i].m_friction = m_Wheels[i].WheelComponent->Friction;
		data.m_wheelParams[i].m_slipAngle = m_Wheels[i].WheelComponent->SlipAngle;

		// This value is also used to calculate the m_primaryTransmissionRatio.
		data.m_wheelParams[i].m_radius = m_Wheels[i].WheelComponent->Radius;
		data.m_wheelParams[i].m_width = m_Wheels[i].WheelComponent->Width;
		data.m_wheelParams[i].m_mass = m_Wheels[i].WheelComponent->Mass;


		// May be in wheelcomponent later
		data.m_wheelParams[i].m_viscosityFriction = 0.25f;
		data.m_wheelParams[i].m_maxFriction = 2.0f * data.m_wheelParams[i].m_friction;
		data.m_wheelParams[i].m_forceFeedbackMultiplier = 0.1f;
		data.m_wheelParams[i].m_maxContactBodyAcceleration = hkReal(data.m_gravity.length3()) * 2;
	}

	
		
	
}

void VehicleSetup::setupComponent(const hkpVehicleData& data, hkpVehicleDefaultAnalogDriverInput& driverInput)
{
	// We also use an analog "driver input" class to help converting user input to vehicle behavior.

	driverInput.m_slopeChangePointX = 0.8f;
	driverInput.m_initialSlope = 0.7f;
	driverInput.m_deadZone = 0.0f;
	driverInput.m_autoReverse = true;
}

void VehicleSetup::setupComponent(const hkpVehicleData& data, hkpVehicleDefaultSteering& steering, Components::Vehicle vehicleComponent )
{
	steering.m_doesWheelSteer.setSize(data.m_numWheels);

	// degrees
	steering.m_maxSteeringAngle = vehicleComponent.MaxSteeringAngle * (HK_REAL_PI / 180);

	// [mph/h] The steering angle decreases linearly 
	// based on your overall max speed of the vehicle. 
	steering.m_maxSpeedFullSteeringAngle = 70.0f * (1.605f / 3.6f); //MPH???!

	for (int i = 0; i < m_Wheels.size(); i++)
	{
		steering.m_doesWheelSteer[i] = m_Wheels[i].WheelComponent->Steering;
	}
}

void VehicleSetup::setupComponent(const hkpVehicleData& data, hkpVehicleDefaultEngine& engine, Components::Vehicle vehicleComponent)
{
	engine.m_maxTorque = vehicleComponent.MaxTorque;

	engine.m_minRPM = vehicleComponent.MinRPM;
	engine.m_optRPM = vehicleComponent.OptimalRPM;

	// This value is also used to calculate the m_primaryTransmissionRatio.
	engine.m_maxRPM = vehicleComponent.MaxRPM;



	engine.m_torqueFactorAtMinRPM = 0.8f;
	engine.m_torqueFactorAtMaxRPM = 0.8f;
	engine.m_resistanceFactorAtMinRPM = 0.05f;
	engine.m_resistanceFactorAtOptRPM = 0.1f;
	engine.m_resistanceFactorAtMaxRPM = 0.3f;
}

void VehicleSetup::setupComponent(const hkpVehicleData& data, hkpVehicleDefaultTransmission& transmission, Components::Vehicle vehicleComponent )
{
	int numberOfGears = 4;
	transmission.m_gearsRatio.setSize(numberOfGears);
	transmission.m_wheelsTorqueRatio.setSize(data.m_numWheels);

	transmission.m_downshiftRPM = 3500.0f;
	transmission.m_upshiftRPM = 6500.0f;

	transmission.m_clutchDelayTime = 0.0f;
	transmission.m_reverseGearRatio = 1.0f;
	transmission.m_gearsRatio[0] = 2.0f;
	transmission.m_gearsRatio[1] = 1.5f;
	transmission.m_gearsRatio[2] = 1.0f;
	transmission.m_gearsRatio[3] = 0.75f;
	transmission.m_wheelsTorqueRatio[0] = 0.2f;
	transmission.m_wheelsTorqueRatio[1] = 0.2f;
	transmission.m_wheelsTorqueRatio[2] = 0.3f;
	transmission.m_wheelsTorqueRatio[3] = 0.3f;


	transmission.m_primaryTransmissionRatio = hkpVehicleDefaultTransmission::calculatePrimaryTransmissionRatio(
		vehicleComponent.TopSpeed,
		m_Wheels[0].WheelComponent->Radius, // HACK: All wheels are the same size right?
		vehicleComponent.MaxRPM,
		transmission.m_gearsRatio[numberOfGears - 1]);
}

void VehicleSetup::setupComponent(const hkpVehicleData& data, hkpVehicleDefaultBrake& brake, Components::Vehicle vehicleComponent )
{
	brake.m_wheelBrakingProperties.setSize(data.m_numWheels);

	for (int i = 0; i < m_Wheels.size(); i++)
	{
		brake.m_wheelBrakingProperties[i].m_maxBreakingTorque = m_Wheels[i].WheelComponent->MaxBreakingTorque;
		brake.m_wheelBrakingProperties[i].m_isConnectedToHandbrake = m_Wheels[i].WheelComponent->ConnectedToHandbrake;

		brake.m_wheelBrakingProperties[i].m_minPedalInputToBlock = 0.9f;
	}

	brake.m_wheelsMinTimeToBlock = 1000.0f;
}

void VehicleSetup::setupComponent(const hkpVehicleData& data, hkpVehicleDefaultSuspension& suspension, Components::Vehicle vehicleComponent)
{
	suspension.m_wheelParams.setSize(data.m_numWheels);
	suspension.m_wheelSpringParams.setSize(data.m_numWheels);

	for (int i = 0; i < m_Wheels.size(); i++)
	{
		float suspensionLength = glm::length(m_Wheels[i].TransformComponent->Position - m_Wheels[i].WheelComponent->Hardpoint);
		suspension.m_wheelParams[i].m_length = suspensionLength;
		suspension.m_wheelSpringParams[i].m_strength  = m_Wheels[i].WheelComponent->SuspensionStrength;

		const float wd = 3.0f;
		suspension.m_wheelSpringParams[i].m_dampingCompression = wd;
		suspension.m_wheelSpringParams[i].m_dampingRelaxation = wd;


		suspension.m_wheelParams[i].m_hardpointChassisSpace.set(m_Wheels[i].WheelComponent->Hardpoint.x, m_Wheels[i].WheelComponent->Hardpoint.y, m_Wheels[i].WheelComponent->Hardpoint.z);
		
		suspension.m_wheelParams[0].m_directionChassisSpace = hkVector4(m_Wheels[i].WheelComponent->DownDirection.x, m_Wheels[i].WheelComponent->DownDirection.y, m_Wheels[i].WheelComponent->DownDirection.z);
	}
}

void VehicleSetup::setupComponent(const hkpVehicleData& data, hkpVehicleDefaultAerodynamics& aerodynamics, Components::Vehicle vehicleComponent )
{
	aerodynamics.m_airDensity = 1.3f;
	// In m^2.
	aerodynamics.m_frontalArea = 1.0f;

	aerodynamics.m_dragCoefficient = 0.7f;
	aerodynamics.m_liftCoefficient = -0.3f;

	// Extra gavity applies in world space (independent of m_chassisCoordinateSystem).
	aerodynamics.m_extraGravityws.set(0.0f, -5.0f, 0.0f);
}

void VehicleSetup::setupComponent(const hkpVehicleData& data, hkpVehicleDefaultVelocityDamper& velocityDamper, Components::Vehicle vehicleComponent)
{
	// Caution: setting negative damping values will add energy to system. 
	// Setting the value to 0 will not affect the angular velocity. 

	// Damping the change of the chassis angular velocity when below m_collisionThreshold. 
	// This will affect turning radius and steering.
	velocityDamper.m_normalSpinDamping = 0.0f;

	// Positive numbers dampen the rotation of the chassis and 
	// reduce the reaction of the chassis in a collision. 
	velocityDamper.m_collisionSpinDamping = 4.0f;

	// The threshold in m/s at which the algorithm switches from 
	// using the normalSpinDamping to the collisionSpinDamping. 	
	velocityDamper.m_collisionThreshold = 1.0f;
}

void VehicleSetup::setupWheelCollide(const hkpWorld* world, const hkpVehicleInstance& vehicle, hkpVehicleRayCastWheelCollide& wheelCollide)
{
	// Set the wheels to have the same collision filter info as the chassis.
	wheelCollide.m_wheelCollisionFilterInfo = vehicle.getChassis()->getCollisionFilterInfo();
}

