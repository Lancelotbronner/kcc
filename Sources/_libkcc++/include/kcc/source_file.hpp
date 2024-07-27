//
//  SourceFile.hpp
//  
//
//  Created by Christophe Bronner on 2024-06-27.
//

namespace kcc {
	class SourceFile {
		char const *_begin;
		char const *_end;

	public:
		SourceFile(SourceFile const &) = delete;
		SourceFile &operator =(SourceFile const &) = delete;

		SourceFile(SourceFile &&other) noexcept = default;
		SourceFile &operator =(SourceFile &&other) noexcept = default;

		explicit SourceFile(char const *filename) noexcept;
	};
}
