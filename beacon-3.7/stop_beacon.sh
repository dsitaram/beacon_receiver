#!/bin/bash

kill `ps ax |grep beacon.py |grep -v grep |awk '{print $1}' |xargs`
kill `ps ax |grep beacon |grep LC_ALL| grep -v grep |awk '{print $1}' |xargs`