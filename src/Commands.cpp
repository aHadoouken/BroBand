#include "Commands.h"

Commands::Commands() {}

void Commands::run() { exec(); }

void Commands::disconnect() { exit(0); }
