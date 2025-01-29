//
//  specifiers.h
//  kcc2
//
//  Created by Christophe Bronner on 2025-01-27.
//

#pragma once

struct specifiers {
	/// Whether the next variable has automatic storage or uses type inference.
	bool is_auto : 1;
	/// Whether the next declaration is a compile-time constant.
	bool is_constexpr : 1;
	/// Whether the next variable has extern storage.
	bool is_extern : 1;
	/// Whether the next variable has register storage.
	bool is_register : 1;
	/// Whether the next variable has static storage.
	bool is_static : 1;
	/// Whether the next variable has thread local storage.
	bool is_thread_local : 1;
	/// Whether the next we're about to parse a typedef.
	bool is_typedef : 1;

	/// Whether the next type is const-qualified
	bool is_const : 1;
	/// Whether the next type is restrict-qualified
	bool is_restrict : 1;
	/// Whether the next type is volatile-qualified
	bool is_volatile : 1;
	/// Whether the next type is atomic-qualified
	bool is_atomic : 1;

	/// Whether the next function is inline
	bool is_inline : 1;
	/// Whether the next function is no-return (deprecated)
	bool is_noreturn : 1;
};
