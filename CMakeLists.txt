cmake_minimum_required(VERSION 3.16)

project(fins_rov_proto LANGUAGES CXX)

fins_add_node(${PROJECT_NAME}
  nodes.cpp
  protocol.cpp
)
