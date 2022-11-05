#include <algorithm> 
#include <cctype>
#include <locale>

namespace Narrator
{
	namespace Runtime
	{
		class StringUtils
		{
		public:
			// trim from start (in place)
			static inline void LeftTrim(std::string& s) {
				s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
					return !std::isspace(ch);
					}));
			}

			// trim from end (in place)
			static inline void RightTrim(std::string& s) {
				s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
					return !std::isspace(ch);
					}).base(), s.end());
			}

			// trim from both ends (in place)
			static inline void Ttrim(std::string& s) {
				LeftTrim(s);
				RightTrim(s);
			}

			// trim from start (copying)
			static inline std::string LeftTrimCopy(std::string s) {
				LeftTrim(s);
				return s;
			}

			// trim from end (copying)
			static inline std::string RightTrimCopy(std::string s) {
				RightTrim(s);
				return s;
			}

			// trim from both ends (copying)
			static inline std::string TrimCopy(std::string s) {
				Ttrim(s);
				return s;
			}

		private:

		};		
	}
}