#!/bin/python

env = Environment()
libs = []

libs += env.Object('SOIUtility.c')
libs += env.Object('producer.c')
libs += env.Object('client.c')

app = env.Program('app', 'main.c', LIBS=libs)



