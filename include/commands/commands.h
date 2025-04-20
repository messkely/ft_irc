#ifndef commands_HPP
# define commands_HPP

// classes
# include "Pass.hpp"
# include "Nick.hpp"
# include "User.hpp"
# include "Join.hpp"
# include "Mode.hpp"
# include "Part.hpp"
# include "Kick.hpp"
# include "Topic.hpp"

// utils
#  define CMDS_N 8
#  define NORESP ""

// names
#  define PASS "PASS"
#  define NICK "NICK"
#  define USER "USER"
#  define JOIN "JOIN"
#  define MODE "MODE"
#  define PART "PART"
#  define TOPIC "TOPIC"
#  define KICK "KICK"

// args number (cmd name included)
# define PASS_ARGS_N 2
# define NICK_ARGS_N 2
# define USER_ARGS_N 5

#endif