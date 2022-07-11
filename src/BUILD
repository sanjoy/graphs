cc_library(
    name = "graph",
    srcs = ["graph.cpp"],
    hdrs = ["graph.hpp"],
    deps = [":logging"],
)

cc_library(
    name = "random",
    srcs = ["random.cpp"],
    hdrs = ["random.hpp"],
)

cc_library(
    name = "graph_analysis",
    srcs = ["graph_analysis.cpp"],
    hdrs = ["graph_analysis.hpp"],
    deps = [":graph", ":random", ":logging"]
)

cc_library(
    name = "random_graph",
    srcs = ["random_graph.cpp"],
    hdrs = ["random_graph.hpp"],
    deps = [":graph", ":random", ":logging"]
)

cc_library(
    name = "graph_zoo",
    srcs = ["graph_zoo.cpp"],
    hdrs = ["graph_zoo.hpp"],
    deps = [":graph", ":graph_analysis", ":logging"]
)

cc_library(
    name = "logging",
    srcs = ["logging.cpp"],
    hdrs = ["logging.hpp"],
)

cc_library(
    name = "counting",
    srcs = ["counting.cpp"],
    hdrs = ["counting.hpp"],
    deps = [":logging"],
)

cc_library(
    name = "graph_viz",
    srcs = ["graph_viz.cpp"],
    hdrs = ["graph_viz.hpp"],
    deps = [":graph"]
)

cc_binary(
    name = "graph_viz_driver",
    srcs = ["graph_viz_driver.cpp"],
    deps = [":graph_viz", ":graph_zoo", ":random_graph"]
)

cc_library(
    name = "test",
    srcs = ["test.cpp"],
    hdrs = ["test.hpp"],
    deps = [":logging", ":graph"],
    testonly = True,
)

cc_test(
    name = "graph_test",
    srcs = ["graph_test.cpp"],
    deps = [":graph", ":test"]
)

cc_test(
    name = "graph_analysis_test",
    srcs = ["graph_analysis_test.cpp"],
    deps = [":graph_analysis", ":random_graph", ":test"]
)

cc_test(
    name = "random_test",
    srcs = ["random_test.cpp"],
    deps = [":random", ":test"]
)

cc_test(
    name = "random_graph_test",
    srcs = ["random_graph_test.cpp"],
    deps = [":random_graph", ":test"]
)

cc_test(
    name = "graph_zoo_test",
    srcs = ["graph_zoo_test.cpp"],
    deps = [":graph_zoo", ":test", ":graph_viz"]
)

cc_test(
    name = "graph_viz_test",
    srcs = ["graph_viz_test.cpp"],
    deps = [":graph_viz", ":test", ":graph_zoo"]
)

cc_test(
    name = "counting_test",
    srcs = ["counting_test.cpp"],
    deps = [":counting", ":test"]
)
