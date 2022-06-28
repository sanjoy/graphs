cc_library(
    name = "graph",
    srcs = ["graph.cpp"],
    hdrs = ["graph.hpp"],
)

cc_library(
    name = "random",
    srcs = ["random.cpp"],
    hdrs = ["random.hpp"],
)

cc_library(
    name = "analyses",
    srcs = ["analyses.cpp"],
    hdrs = ["analyses.hpp"],
    deps = [":graph", ":random"]
)


cc_library(
    name = "test",
    srcs = ["test.cpp"],
    hdrs = ["test.hpp"],
    testonly = True,
)

cc_test(
    name = "graph_test",
    srcs = ["graph_test.cpp"],
    deps = [":graph", ":test"]
)

cc_test(
    name = "analyses_test",
    srcs = ["analyses_test.cpp"],
    deps = [":analyses", ":test"]
)
