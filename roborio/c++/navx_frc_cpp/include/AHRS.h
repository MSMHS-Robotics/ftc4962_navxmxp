/*
 * AHRS.h
 *
 *  Created on: Jul 30, 2015
 *      Author: Scott
 */

#ifndef SRC_AHRS_H_
#define SRC_AHRS_H_

#include "WPILib.h"

class IIOProvider;
class ContinuousAngleTracker;
class InertialDataIntegrator;
class OffsetTracker;
class AHRSInternal;

class AHRS : public SensorBase,
             public LiveWindowSendable,
             public PIDSource  {
public:

    enum BoardAxis {
        kBoardAxisX = 0,
        kBoardAxisY = 1,
        kBoardAxisZ = 2,
    };

    struct BoardYawAxis
    {
        /* Identifies one of the board axes */
        BoardAxis board_axis;
        /* true if axis is pointing up (with respect to gravity); false if pointing down. */
        bool up;
    };

    enum SerialDataType {
    /**
     * (default):  6 and 9-axis processed data
     */
    kProcessedData = 0,
    /**
     * unprocessed data from each individual sensor
     */
    kRawData = 1
    };

private:
    friend class AHRSInternal;
    AHRSInternal *      ahrs_internal;

    volatile float      yaw;
    volatile float      pitch;
    volatile float      roll;
    volatile float      compass_heading;
    volatile float      world_linear_accel_x;
    volatile float      world_linear_accel_y;
    volatile float      world_linear_accel_z;
    volatile float      mpu_temp_c;
    volatile float      fused_heading;
    volatile float      altitude;
    volatile float      baro_pressure;
    volatile bool       is_moving;
    volatile bool       is_rotating;
    volatile float      baro_sensor_temp_c;
    volatile bool       altitude_valid;
    volatile bool       is_magnetometer_calibrated;
    volatile bool       magnetic_disturbance;
    volatile int16_t    quaternionW;
    volatile int16_t    quaternionX;
    volatile int16_t    quaternionY;
    volatile int16_t    quaternionZ;

    /* Integrated Data */
    float velocity[3];
    float displacement[3];


    /* Raw Data */
    volatile int16_t    raw_gyro_x;
    volatile int16_t    raw_gyro_y;
    volatile int16_t    raw_gyro_z;
    volatile int16_t    raw_accel_x;
    volatile int16_t    raw_accel_y;
    volatile int16_t    raw_accel_z;
    volatile int16_t    cal_mag_x;
    volatile int16_t    cal_mag_y;
    volatile int16_t    cal_mag_z;

    /* Configuration/Status */
    volatile uint8_t    update_rate_hz;
    volatile int16_t    accel_fsr_g;
    volatile int16_t    gyro_fsr_dps;
    volatile int16_t    capability_flags;
    volatile uint8_t    op_status;
    volatile int16_t    sensor_status;
    volatile uint8_t    cal_status;
    volatile uint8_t    selftest_status;

    /* Board ID */
    volatile uint8_t    board_type;
    volatile uint8_t    hw_rev;
    volatile uint8_t    fw_ver_major;
    volatile uint8_t    fw_ver_minor;

    long                last_sensor_timestamp;
    double              last_update_time;

    ITable *            table;

    InertialDataIntegrator *integrator;
    ContinuousAngleTracker *yaw_angle_tracker;
    OffsetTracker *         yaw_offset_tracker;
    IIOProvider *           io;

    Task *                  task;

public:
    AHRS(SPI::Port spi_port_id);
    AHRS(I2C::Port i2c_port_id);
    AHRS(SerialPort::Port serial_port_id);

    AHRS(SPI::Port spi_port_id, uint8_t update_rate_hz);
    AHRS(SPI::Port spi_port_id, uint32_t spi_bitrate, uint8_t update_rate_hz);

    AHRS(I2C::Port i2c_port_id, uint8_t update_rate_hz);

    AHRS(SerialPort::Port serial_port_id, AHRS::SerialDataType data_type, uint8_t update_rate_hz);

    float  GetPitch();
    float  GetRoll();
    float  GetYaw();
    float  GetCompassHeading();
    void   ZeroYaw();
    bool   IsCalibrating();
    bool   IsConnected();
    double GetByteCount();
    double GetUpdateCount();
    float  GetWorldLinearAccelX();
    float  GetWorldLinearAccelY();
    float  GetWorldLinearAccelZ();
    bool   IsMoving();
    bool   IsRotating();
    float  GetBarometricPressure();
    float  GetAltitude();
    bool   IsAltitudeValid();
    float  GetFusedHeading();
    bool   IsMagneticDisturbance();
    bool   IsMagnetometerCalibrated();
    float  GetQuaternionW();
    float  GetQuaternionX();
    float  GetQuaternionY();
    float  GetQuaternionZ();
    void   ResetDisplacement();
    void   UpdateDisplacement( float accel_x_g, float accel_y_g,
                               int update_rate_hz, bool is_moving );
    float  GetVelocityX();
    float  GetVelocityY();
    float  GetVelocityZ();
    float  GetDisplacementX();
    float  GetDisplacementY();
    float  GetDisplacementZ();
    double GetAngle();
    double GetRate();
    void   Reset();
    float  GetRawGyroX();
    float  GetRawGyroY();
    float  GetRawGyroZ();
    float  GetRawAccelX();
    float  GetRawAccelY();
    float  GetRawAccelZ();
    float  GetRawMagX();
    float  GetRawMagY();
    float  GetRawMagZ();
    float  GetPressure();
    float  GetTempC();
    AHRS::BoardYawAxis GetBoardYawAxis();
    std::string GetFirmwareVersion();

private:
    void SPIInit( SPI::Port spi_port_id, uint32_t bitrate, uint8_t update_rate_hz );
    void I2CInit( I2C::Port i2c_port_id, uint8_t update_rate_hz );
    void SerialInit(SerialPort::Port serial_port_id, AHRS::SerialDataType data_type, uint8_t update_rate_hz);
    void commonInit( uint8_t update_rate_hz );
    static int ThreadFunc(IIOProvider *io_provider);

    /* LiveWindowSendable implementation */
    void InitTable(ITable* subtable);
    ITable* GetTable();
    std::string GetSmartDashboardType();
    void UpdateTable();
    void StartLiveWindowMode();
    void StopLiveWindowMode();

    /* PIDSource implementation */
    double PIDGet();
};

#endif /* SRC_AHRS_H_ */
