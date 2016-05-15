#!/bin/bash

aplay -q "$1"  &
disown
