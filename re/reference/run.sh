#!/bin/sh
# Starts the reference environment (Windows 95 under DOSBox-X). Run from anywhere; paths in the config are relative to the repository root.
cd "$(dirname "$0")/../.."
exec dosbox-x -conf re/reference/dosbox-x.conf "$@"
