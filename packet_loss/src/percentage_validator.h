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

            CLI::detail::rtrim(input);
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
                CLI::detail::rtrim(input);
            }

            if (input.empty()) {
                throw CLI::ValidationError("Input is empty");
            }

            if (!has_percent) {
                if (!CLI::detail::lexical_cast(input, num)) {
                    throw CLI::ValidationError(std::string("Value ") + input + " could not be converted to double");
                }
                // No need to modify input if no unit passed
                return {};
            }

            bool converted = CLI::detail::lexical_cast(input, num);
            if (!converted) {
                throw CLI::ValidationError(std::string("Value ") + input + " could not be converted to double");
            }
            num = num / 100.0;

            input = CLI::detail::to_string(num);

            return {};
        };
    }

   private:
    /// Generate description like this: NUMBER [UNIT]
    static std::string generate_description() {
        std::stringstream out;
        out << "double [%]";
        return out.str();
    }
};

#endif
