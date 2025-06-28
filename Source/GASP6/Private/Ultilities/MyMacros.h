#pragma once

#define CLAMP(current, max, min) (current >= max ? max : (current <= min ? min : current))