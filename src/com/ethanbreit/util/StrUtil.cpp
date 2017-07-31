//
// Created by Ethan Breit on 2017-07-13.
//
#include <util/StrUtil.h>
namespace ge {
    namespace StringUtil {

        void removeChar(std::string *str, char rem) {
            for (unsigned int i = 0; i < str->length(); i++) {
                if ((*str)[i] == rem)
                    str->erase(i);
            }
        }


        int amountOfChar(std::string str, char chr) {
            int amount = 0;
            for (unsigned int i = 0; i < str.length(); i++) {
                if (str[i] == chr)
                    amount++;
            }
            return amount;

        }
    }
}