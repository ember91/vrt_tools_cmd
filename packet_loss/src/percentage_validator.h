#ifndef VRT_PACKET_LOSS_SRC_PERCENTAGE_VALIDATOR_H_
#define VRT_PACKET_LOSS_SRC_PERCENTAGE_VALIDATOR_H_

#include "CLI/Validators.hpp"

/**
 * Ensure that ratios with a percentage symbol after are parsed accordingly.
 */
class PercentageValidator : public CLI::Validator {
   public:
    explicit PercentageValidator() {
        description(generate_description());

        // transform function
        func_ = [](std::string& input) -> std::string {
            double num;

            rtrim(input);
            if (input.empty()) {
                throw CLI::ValidationError("Input is empty");
            }

            // Find split position between number and prefix
            bool has_percent{false};
            if (*(input.end() - 1) == '%') {
                has_percent = true;
            }

            if (has_percent) {
                input.resize(static_cast<std::size_t>(std::distance(input.begin(), input.end() - 1)));
                rtrim(input);
            }

            if (input.empty()) {
                throw CLI::ValidationError("Input is empty");
            }

            try {
                num = std::stod(input);
            } catch (const std::invalid_argument&) {
                throw CLI::ValidationError(std::string("Value ") + input + " could not be converted to double");
            }

            if (!has_percent) {
                // No need to modify input if no unit passed
                return {};
            }

            num = num / 100.0;

            input = std::to_string(num);

            return {};
        };
    }

   private:
    /**
     * Trim whitespace from right of string.
     */
    static std::string& rtrim(std::string& str) {
        auto it =
            std::find_if(str.rbegin(), str.rend(), [](char ch) { return !std::isspace<char>(ch, std::locale()); });
        str.erase(it.base(), str.end());
        return str;
    }

    /**
     * Generate description like this: NUMBER [UNIT]
     */
    static std::string generate_description() {
        std::stringstream out;
        out << "double [%]";
        return out.str();
    }
};

#endif
