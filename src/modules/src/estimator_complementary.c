
#include "stabilizer.h"
#include "stabilizer_types.h"

#include "sensfusion6.h"
#include "position_estimator.h"

#define ATTITUDE_UPDATE_HZ 250
#define ATTITUDE_UPDATE_DT 1.0/ATTITUDE_UPDATE_HZ

#define POS_UPDATE_HZ 100
#define POS_UPDATE_DT 1.0/POS_UPDATE_HZ

void stateEstimator(state_t *state, const sensorData_t *sensorData)
{
  if (!RATE_SKIP_250HZ()) {
    sensfusion6UpdateQ(sensorData->gyro.x, sensorData->gyro.y, sensorData->gyro.z,
                       sensorData->acc.x, sensorData->acc.y, sensorData->acc.z,
                       ATTITUDE_UPDATE_DT);
    sensfusion6GetEulerRPY(&state->attitude.roll, &state->attitude.pitch, &state->attitude.yaw);

    state->acc.z = sensfusion6GetAccZWithoutGravity(sensorData->acc.x,
                                                    sensorData->acc.y,
                                                    sensorData->acc.z);

    positionUpdateVelocity(state->acc.z, ATTITUDE_UPDATE_DT);
  }

  if (!RATE_SKIP_100HZ()) {
    positionEstimate(state, sensorData->baro.asl, POS_UPDATE_DT);
  }
}
