#include "AbstractController.h"


AbstractController::AbstractController(ConfigController* configController, AbstractDevice device)
{
  Serial.println("Constructing the Abastract Controller");
  this->config = configController->config;
  this->motorController = new MotorController; // Question:: Does this mean I have a new motorController object for each instance of the AbstractController?
  this->motorController->setup();

  this->controller = device; // TODO:: why do we store the device on the abstract and the concrete controller?
  this->setup();
}

void AbstractController::setup()
{
  // Setting the pointers to Motor and Wifi.
  //this->motorController = motorController;
  Serial.println("Controller is setting up the CurrentController");
  currentController.setup(config);
  // Seting the default values and states for the controller;
  defaultInputNeutral         = config->controller.defaultInputNeutral;
  defaultInputMinBrake        = config->controller.defaultInputMinBrake;
  defaultInputMaxBrake        = config->controller.defaultInputMaxBrake;
  defaultInputMinAcceleration = config->controller.defaultInputMinAcceleration;
  defaultInputMaxAcceleration = config->controller.defaultInputMaxAcceleration;
  defaultSmoothAlpha          = config->controller.defaultSmoothAlpha;

  // Setting inputs to neutral;
  latestInput                 = defaultInputNeutral;
  previousInput               = defaultInputNeutral;

}

// Decides the state of the motor
bool AbstractController::setMotorPower()
{

  if (previousInput > defaultInputMinBrake && previousInput < defaultInputMinAcceleration)
  {
    // we are in neutral.
    float motorCurrent = currentController.getNeutralCurrent();
    motorController->set_current(motorCurrent);

    return 0;
  }

  if (previousInput >= defaultInputMinAcceleration)
  {
    //accelerating
    float motorCurrent = currentController.getMotorAccelerationCurrent(previousInput);
    motorController->set_current(motorCurrent);

    return 1;
  }

  // braking;
  float motorCurrent = currentController.getMotorBrakingCurrent(previousInput);
  motorController->set_current_brake(motorCurrent);

  return 1;
}

void AbstractController::processInput(byte latestInput)
{
  float targetAlpha = (defaultSmoothAlpha * latestInput) + ((1 - defaultSmoothAlpha * previousInput));
  // If the value is close to target, set it to target
  if(abs(float(latestInput) - float(targetAlpha)))
  {
    targetAlpha = latestInput;
  }
  if (latestInput > defaultInputMinBrake && latestInput < defaultInputMinAcceleration)
  {
    // When within the neutral boundary, set the default neutral value
    latestInput = defaultInputNeutral;
  }
  previousInput = constrain(latestInput, defaultInputMaxBrake, defaultInputMaxAcceleration);
  AbstractController::setMotorPower();
}
