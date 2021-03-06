#include "graph_analysis.hpp"
#include "graph_viz.hpp"
#include "graph_zoo.hpp"
#include "random_graph.hpp"

#include <cctype>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace kb {
namespace {
void ltrim(std::string *s) {
  s->erase(s->begin(), std::find_if(s->begin(), s->end(), [](unsigned char ch) {
             return !std::isspace(ch);
           }));
}

void rtrim(std::string *s) {
  s->erase(std::find_if(s->rbegin(), s->rend(),
                        [](unsigned char ch) { return !std::isspace(ch); })
               .base(),
           s->end());
}

void trim(std::string *s) {
  ltrim(s);
  rtrim(s);
}

std::vector<std::string> SplitIntoWords(const std::string &s) {
  std::istringstream iss(s);
  std::string item;
  std::vector<std::string> words;
  while (std::getline(iss, item, ' '))
    words.push_back(item);
  return words;
}

std::optional<long> StrToL(const std::string &s) {
  char *end;
  auto result = std::strtol(s.c_str(), &end, 10);
  if ((end - s.c_str()) != s.size())
    return std::nullopt;
  return result;
}

class Repl {
public:
  using Error = std::string;
  using GraphResult = std::variant<std::unique_ptr<Graph>, Error>;

  static constexpr int kAssignOpOffset = 1;

  GraphResult MakeCompleteGraph(const std::string &cmd,
                                const std::vector<std::string> &cmd_words,
                                bool *matched) {
    *matched = cmd_words[kAssignOpOffset + 1] == "complete";
    if (!*matched)
      return std::unique_ptr<Graph>(nullptr);

    if (cmd_words.size() != kAssignOpOffset + 3)
      return "Expected command of the form \"x = complete 5\", got \"" + cmd +
             "\"";

    auto maybe_k = StrToL(cmd_words[kAssignOpOffset + 2]);
    if (!maybe_k)
      return "Expected command of the form \"x = complete 5\", got \"" + cmd +
             "\"";

    return CreateCompleteGraph(*maybe_k, false);
  }

  GraphResult MakeUnconnectedGraph(const std::string &cmd,
                                   const std::vector<std::string> &cmd_words,
                                   bool *matched) {
    *matched = cmd_words[kAssignOpOffset + 1] == "unconnected";
    if (!*matched)
      return std::unique_ptr<Graph>(nullptr);

    if (cmd_words.size() != kAssignOpOffset + 3)
      return "Expected command of the form \"x = unconnected 5\", got \"" +
             cmd + "\"";

    auto maybe_k = StrToL(cmd_words[kAssignOpOffset + 2]);
    if (!maybe_k)
      return "Expected command of the form \"x = complete 5\", got \"" + cmd +
             "\"";

    return CreateUnconnectedGraph(*maybe_k);
  }

  GraphResult MakeBipartiteGraph(const std::string &cmd,
                                 const std::vector<std::string> &cmd_words,
                                 bool *matched) {
    *matched = cmd_words[kAssignOpOffset + 1] == "complete_bipartite";
    if (!*matched)
      return std::unique_ptr<Graph>(nullptr);

    if (cmd_words.size() != kAssignOpOffset + 4)
      return "Expected command of the form \"x = complete_bipartite 2 3\", got "
             "\"" +
             cmd + "\"";

    auto maybe_l = StrToL(cmd_words[kAssignOpOffset + 2]);
    auto maybe_r = StrToL(cmd_words[kAssignOpOffset + 2]);
    if (!maybe_l || !maybe_r)
      return "Expected command of the form \"x = complete_bipartite 2 3\", got "
             "\"" +
             cmd + "\"";

    return CreateCompleteBipartiteGraph(*maybe_l, *maybe_r);
  }

  GraphResult
  MakeReplacementProductGraph(const std::string &cmd,
                              const std::vector<std::string> &cmd_words,
                              bool *matched) {
    *matched = cmd_words[kAssignOpOffset + 1] == "replacement_product";
    if (!*matched)
      return std::unique_ptr<Graph>(nullptr);

    if (cmd_words.size() != kAssignOpOffset + 4)
      return "Expected command of the form \"x = replacement_product <outer "
             "graph> <inner graph>\", got \"" +
             cmd + "\"";

    const std::string &outer_graph_name = cmd_words[kAssignOpOffset + 2];
    auto outer_graph_it = graphs_.find(outer_graph_name);
    if (outer_graph_it == graphs_.end())
      return "Could not find outer graph " + outer_graph_name;
    std::unique_ptr<Graph> outer_graph = outer_graph_it->second->Clone();
    auto outer_graph_degree = IsRegular(outer_graph.get());
    if (!outer_graph_degree.has_value())
      return "Outer graph " + outer_graph_name + " is not regular";

    const std::string &inner_graph_name = cmd_words[kAssignOpOffset + 3];
    auto inner_graph_it = graphs_.find(inner_graph_name);
    if (inner_graph_it == graphs_.end())
      return "Could not find inner graph " + inner_graph_name;
    std::unique_ptr<Graph> inner_graph = inner_graph_it->second->Clone();

    if (*outer_graph_degree != inner_graph->GetOrder())
      return "Outer graph degree " + std::to_string(*outer_graph_degree) +
             " does not match inner graph order " +
             std::to_string(inner_graph->GetOrder());

    return CreateReplacementProduct(std::move(outer_graph),
                                    std::move(inner_graph));
  }

  GraphResult MakeRandomGraph(const std::string &cmd,
                              const std::vector<std::string> &cmd_words,
                              bool *matched) {
    *matched = cmd_words[kAssignOpOffset + 1] == "random";
    if (!*matched)
      return std::unique_ptr<Graph>(nullptr);

    std::string error_msg = "Expected command of the form \"x = random <order> "
                            "<avg degree> <optional seed>, got \"" +
                            cmd + "\"";

    if (cmd_words.size() != (kAssignOpOffset + 4) &&
        cmd_words.size() != (kAssignOpOffset + 5))
      return error_msg;

    auto maybe_order = StrToL(cmd_words[kAssignOpOffset + 2]);
    if (!maybe_order)
      return error_msg;

    auto maybe_avg_degree = StrToL(cmd_words[kAssignOpOffset + 3]);
    if (!maybe_avg_degree)
      return error_msg;

    unsigned seed = 1;

    if (cmd_words.size() == (kAssignOpOffset + 5)) {
      auto maybe_seed = StrToL(cmd_words[kAssignOpOffset + 4]);
      if (!maybe_seed)
        return error_msg;
      seed = *maybe_seed;
    }

    auto rng = CreateDefaultRandomBitGenerator(seed);
    return CreateRandomSparseGraph(rng.get(), *maybe_order, *maybe_avg_degree);
  }

  GraphResult MakeGraph(const std::string &cmd,
                        const std::vector<std::string> &cmd_words) {
    GraphResult result;

#define MAKE_GRAPH_CASE(kind)                                                  \
  do {                                                                         \
    bool matched;                                                              \
    result = Make##kind##Graph(cmd, cmd_words, &matched);                      \
    if (matched)                                                               \
      return result;                                                           \
  } while (0)

    MAKE_GRAPH_CASE(Complete);
    MAKE_GRAPH_CASE(Unconnected);
    MAKE_GRAPH_CASE(Bipartite);
    MAKE_GRAPH_CASE(ReplacementProduct);
    MAKE_GRAPH_CASE(Random);

#undef MAKE_GRAPH_CASE

    return "Cannot parse graph construction RHS in \"" + cmd + "\"";
  }

  std::optional<std::string>
  MakeGraphAndAssign(const std::string &cmd,
                     const std::vector<std::string> &cmd_words, bool *matched) {
    if (cmd_words.size() < 2 || cmd_words[1] != "=") {
      *matched = false;
      return std::nullopt;
    }

    *matched = true;
    auto maybe_graph = MakeGraph(cmd, cmd_words);
    if (auto *error = std::get_if<std::string>(&maybe_graph))
      return *error;

    graphs_[cmd_words[0]] =
        std::move(std::get<std::unique_ptr<Graph>>(maybe_graph));
    return std::nullopt;
  }

  std::optional<std::string>
  VisualizeGraph(const std::string &cmd,
                 const std::vector<std::string> &cmd_words, bool *matched) {
    if (cmd_words.size() != 2 || cmd_words[0] != "viz") {
      *matched = false;
      return std::nullopt;
    }

    *matched = true;
    auto it = graphs_.find(cmd_words[1]);
    if (it == graphs_.end())
      return "Could not find constructed graph \"" + cmd_words[1] + "\"";

    auto result =
        CreatePngViaGraphviz(it->second.get(), "/tmp/graph", /*open=*/true);
    if (!result)
      return "Could not create PNG from \"" + cmd_words[1] + "\"";
    return std::nullopt;
  }

  std::optional<std::string> RunCommand(std::string cmd, bool *exit) {
    trim(&cmd);
    if (cmd == "quit" || cmd == "exit") {
      *exit = true;
      return std::nullopt;
    }

    *exit = false;

    auto words = SplitIntoWords(cmd);

#define RUN_CMD_CASE(fn)                                                       \
  do {                                                                         \
    bool matched;                                                              \
    auto result = fn(cmd, words, &matched);                                    \
    if (matched)                                                               \
      return result;                                                           \
  } while (0)

    RUN_CMD_CASE(MakeGraphAndAssign);
    RUN_CMD_CASE(VisualizeGraph);

    return "\"" + cmd + "\"" + " does not match any commands!";
  }

private:
  std::map<std::string, std::unique_ptr<Graph>> graphs_;
};

void RealMain() {
  bool exit = false;
  Repl repl;
  do {
    std::cout << "> ";

    std::string input;
    if (!std::getline(std::cin, input))
      break; // EOF

    auto result = repl.RunCommand(std::move(input), &exit);
    if (result.has_value())
      std::cout << "Error: " << result.value() << "\n";
  } while (!exit);
}
} // namespace
} // namespace kb

int main() { kb::RealMain(); }
