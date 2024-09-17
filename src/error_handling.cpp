#include <cjf/error_handling.h>
#include <cstring>
#include <system_error>

using namespace cjf;

esp_error::esp_error(const esp_err_t err, const std::source_location loc)
    : std::exception(), error_code(err), location(loc)
{
  strncpy(msg, esp_err_to_name(error_code), 32);
  strcat(msg, " at ");
  strncat(msg, location.file_name(), 32);
  strcat(msg, ":");
  strncat(msg, std::to_string(location.line()).c_str(), 10);
}

const char *cjf::esp_error::what() const noexcept
{
  return msg;
}

void cjf::periodic_log_trap(const periodic_delay_func delay, const char *error_msg)
{
  while (1)
  {
    ESP_LOGE("trap", "%s", error_msg);
    delay();
  }
}

void cjf::sos_trap(const morse_writer &morse_output, const char *error_code)
{
  while (1)
  {
    morse_output.write_word_gap();
    morse_output.write_sos();
    if (error_code)
    {
      morse_output.write_word_gap();
      morse_output.write_string(error_code);
    }
  }
}

void cjf::throw_if_error(const esp_err_t err, const std::source_location loc)
{
  if (err == ESP_OK)
    return;
  throw esp_error(err, loc);
}
