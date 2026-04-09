#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "track_settings.h"
#include "car_setup.h"

int main(void)
{
    Camera_init();
    DC_EN_init();

    uint32_t TIMA1Load = (SYSCTL_SYSCLK_getMCLK() / (9 + 1) / 50) - 1;
    TIMA1_PWM_init(0, TIMA1Load, 9, 92.5);

    uint32_t TIMA0Load = (SYSCTL_SYSCLK_getMCLK() / (0 + 1) / 10000) - 1;

    TIMA0_PWM_init(0, TIMA0Load, 0, 0.0);
    TIMA0_PWM_init(1, TIMA0Load, 0, 0.0);
    TIMA0_PWM_init(2, TIMA0Load, 0, 0.0);
    TIMA0_PWM_init(3, TIMA0Load, 0, 0.0);

    static float filteredServo = 92.5f;
    static int prevError = 0;

    while (1)
    {
        if (!Camera_isDataReady()) continue;

        uint16_t *pixels = Camera_getData();

        // --- CARPET STOP ---
        if (is_carpet(pixels))
        {
            TIMA0_PWM_DutyCycle(0, 0.0);
            TIMA0_PWM_DutyCycle(1, 0.0);
            TIMA0_PWM_DutyCycle(2, 0.0);
            TIMA0_PWM_DutyCycle(3, 0.0);

            TIMA1_PWM_DutyCycle(0, 92.5);

            Camera_clearDataReady();
            continue;
        }

        // --- CENTER LINE ---
        int center = find_center_line(pixels);
        int error = center - 64;

        // Deadband (reduces jitter)
        if (error > -2 && error < 2) error = 0;

        // --- PD CONTROL ---
        float Kp = 0.45f;
        float Kd = 0.35f;

        int derivative = error - prevError;

        float targetServo = 92.5f + (Kp * error) + (Kd * derivative);

        prevError = error;

        // Clamp servo range
        if (targetServo > 100.0f) targetServo = 100.0f;
        if (targetServo < 85.0f)  targetServo = 85.0f;

        // --- RATE LIMITING (smooth but still fast) ---
        float maxStep = 3.0f;

        float delta = targetServo - filteredServo;

        if (delta > maxStep) delta = maxStep;
        if (delta < -maxStep) delta = -maxStep;

        filteredServo += delta;

        TIMA1_PWM_DutyCycle(0, (double)filteredServo);

        // --- SPEED CONTROL ---
        int absError = (error < 0) ? -error : error;

        double baseSpeed = 0.0;
        double maxBoost  = 0.0;

        double normalizedError = (double)absError / 64.0;
        double speed = baseSpeed + maxBoost * (1.0 - (normalizedError * normalizedError));

        TIMA0_PWM_DutyCycle(0, 0.0);
        TIMA0_PWM_DutyCycle(1, speed);
        TIMA0_PWM_DutyCycle(2, 0.0);
        TIMA0_PWM_DutyCycle(3, speed);

        Camera_clearDataReady();
    }
}