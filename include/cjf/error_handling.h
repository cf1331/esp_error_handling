#ifndef FB2B04E1_4B4A_434F_9489_F3D164B49647
#define FB2B04E1_4B4A_434F_9489_F3D164B49647

#include <cjf/morse_writer.h>
#include <esp_check.h>
#include <esp_err.h>
#include <esp_log.h>
#include <exception>
#include <source_location>

namespace cjf
{

/**
 * Macro which can be used to check the error code. If the code is not ESP_OK, it prints the message and returns.
 * Modified from ESP_RETURN_ON_ERROR to include the error name in the log message.
 */
#define LOG_IF_ERROR(x, log_tag)                                                         \
  do                                                                                     \
  {                                                                                      \
    esp_err_t err_rc_ = (x);                                                             \
    if (unlikely(err_rc_ != ESP_OK))                                                     \
    {                                                                                    \
      ESP_LOGE(log_tag, "%s(%d): %s", __FUNCTION__, __LINE__, esp_err_to_name(err_rc_)); \
    }                                                                                    \
  } while (0)

#define RETURN_ON_ERROR(x, log_tag)                                                      \
  do                                                                                     \
  {                                                                                      \
    esp_err_t err_rc_ = (x);                                                             \
    if (unlikely(err_rc_ != ESP_OK))                                                     \
    {                                                                                    \
      ESP_LOGE(log_tag, "%s(%d): %s", __FUNCTION__, __LINE__, esp_err_to_name(err_rc_)); \
      return err_rc_;                                                                    \
    }                                                                                    \
  } while (0)

#define TRAP_ON_ERROR(x)                                                                  \
  do                                                                                      \
  {                                                                                       \
    esp_err_t err_rc_ = (x);                                                              \
    if (unlikely(err_rc_ != ESP_OK))                                                      \
    {                                                                                     \
      while (1)                                                                           \
      {                                                                                   \
        ESP_LOGE("trap", "%s(%d): %s", __FUNCTION__, __LINE__, esp_err_to_name(err_rc_)); \
        vTaskDelay(pdMS_TO_TICKS(1000));                                                  \
      }                                                                                   \
    }                                                                                     \
  } while (0)

  class esp_error : public std::exception
  {
  public:
    const esp_err_t error_code;
    const std::source_location location;

    esp_error(const esp_err_t err, const std::source_location loc = std::source_location::current());
    const char *what() const noexcept override;

  private:
    char msg[80];
  };

  /**
   * @brief Pointer to a function that should block for a period of time.
   */
  typedef void (*periodic_delay_func)();

  /**
   * An infinite loop that Logs an error message periodically.
   *
   * @param delay The function used to introduce a delay between log messages.
   * @param error_msg The error message to be logged.
   */
  void periodic_log_trap(const periodic_delay_func delay, const char *error_msg);

  /**
   * @brief An infinite loop that outputs SOS and an error message in Morse Code.
   *
   * @param writer The Morse Code writer used to output the SOS signal.
   * @param error_code Typically 1-3 ascii characters to identify the error.
   */
  void sos_trap(const morse_writer &writer, const char *error_code = NULL);

  /**
   * Throws an exception if the given error code is not equal to ESP_OK.
   *
   * @param err The error code to check.
   * @param loc The source location where the function is called (defaults to the calling line).
   * @throws cjf::esp_error if the error code is not equal to ESP_OK.
   */
  void throw_if_error(const esp_err_t err, const std::source_location loc = std::source_location::current());

}

#endif /* FB2B04E1_4B4A_434F_9489_F3D164B49647 */
