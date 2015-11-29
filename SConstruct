#!/bin/python

env = Environment()
libs = []

libs += env.Object('SOIUtility.c')
libs += env.Object('Producer.c')
libs += env.Object('Client.c')

app = env.Program('app', 'main.c', LIBS=libs)



