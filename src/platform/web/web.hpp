#ifndef WEB_HPP
#define WEB_HPP
#pragma once

#ifdef SW_WEB
#include <emscripten.h>

EM_JS(int, is_page_visible, (), {
	return document.visibilityState === 'visible' ? 1 : 0;
});

EM_JS(int, get_window_width, (), {
	return window.innerWidth;
});

EM_JS(int, get_window_height, (), {
	return window.innerHeight;
});

#else
#error "can not include platform/web/web.hpp on non-web builds"
#endif

#endif // WEB_HPP