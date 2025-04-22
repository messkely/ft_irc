#include "../include/commands/commands.h"
#include "../include/Channel.hpp"

int main()
{
	Channel *channel;
	std::cout << channel->getUserListStr() << std::endl;
}

Below is a revised, drop‑in replacement for your `Privmsg` implementation that addresses all of the issues we discussed:

```cpp
// In Privmsg.hpp, make sure you have:
//   std::vector<std::string>  channelNames;
//   std::vector<std::string>  nicks;
//   std::string                msg, respStr;
//   // optionally, if you support operator masks:
//   std::vector<std::string>  hostMasks, serverMasks;

void Privmsg::parse()
{
    // ——— 1) allow any number ≥3 so multi‑word messages work
    if (argc < 3)
    {
        respStr = ERR_NEEDMOREPARAMS(name);
        return;
    }

    // ——— 2) split out targets into channels vs. nicks (guard empty tokens)
    std::stringstream ss(args[1]);
    std::string target;
    while (std::getline(ss, target, ','))
    {
        if (target.empty())
            continue;

        char p = target[0];
        // channel mask (#channel or &channel)
        if (p == '#' || p == '&')
            channelNames.push_back(target);
        // server‑mask or host‑mask for ops (e.g. $*.fi or #*.edu)
        else if ((p == '$' || p == '#') &&
                 (target.find('*') != std::string::npos || target.find('?') != std::string::npos))
        {
            // assume operators only; stash for execute()
            if (p == '$')        serverMasks.push_back(target);
            else /* p=='#' */    hostMasks.push_back(target);
        }
        else
        {
            nicks.push_back(target);
        }
    }

    // ——— 3) enforce a maximum‑targets limit (e.g. 5)
    size_t totalTargets = channelNames.size() + nicks.size() + serverMasks.size() + hostMasks.size();
    if (totalTargets > server.getMaxTargets())
    {
        respStr = ERR_TOOMANYTARGETS(args[1]);
        return;
    }

    // ——— 4) assemble the message (strip leading ':' if present)
    msg.clear();
    for (int i = 2; i < argc; ++i)
    {
        if (!args[i] || args[i][0] == '\0')
            continue;
        if (!msg.empty())
            msg += ' ';
        msg += args[i];
    }
    if (!msg.empty() && msg[0] == ':' )
        msg.erase(0, 1);

    // ——— 5) require non‑empty text
    if (msg.empty())
    {
        respStr = ERR_NOTEXTTOSEND(name);
        return;
    }

    respStr = NORESP;
}

void Privmsg::execute()
{
    if (respStr != NORESP)
        return;

    respStr.clear();

    // —— 1) channels
    for (const std::string &chName : channelNames)
    {
        Channel *chan = server.getChannel(chName);
        if (!chan)
        {
            respStr += ERR_NOSUCHCHANNEL(chName);
        }
        else if (!chan->hasUser(&client) && !chan->hasOperator(&client))
        {
            respStr += ERR_CANNOTSENDTOCHAN(client.getNickname(), chName);
        }
        else
        {
            // broadcast the raw message prefixed with client nick
            chan->broadcast(RSP_MSG(client.getNickname(), msg));
        }
    }

    // —— 2) host/server masks (operators only)
    if (!hostMasks.empty() || !serverMasks.empty())
    {
        if (!client.isOperator())
        {
            // no‑privileges error once is enough
            respStr += ERR_NOPRIVILEGES(client.getNickname());
        }
        else
        {
            // dispatch by mask; you need to implement msgByHostMask/ServerMask
            for (const auto &mask : hostMasks)
                server.msgByHostMask(mask, RSP_MSG(client.getNickname(), msg));
            for (const auto &mask : serverMasks)
                server.msgByServerMask(mask, RSP_MSG(client.getNickname(), msg));
        }
    }

    // —— 3) users
    for (const std::string &nick : nicks)
    {
        Client *target = server.findUser(nick);
        if (!target)
        {
            respStr += ERR_NOSUCHNICK(nick);
        }
        else
        {
            // send a PRIVMSG reply to the target client
            *target << RSP_PRIVMSG(client.getNickname(), nick, msg);
        }
    }
}

void Privmsg::resp()
{
    client << respStr;
}

ACommand *Privmsg::create(std::string name, Server &server, Client &client, char **args, int argc)
{
    return new Privmsg(name, server, client, args, argc);
}
```

---

### What’s been fixed or improved

1. **`argc < 3`** instead of `argc != 3` — allows multi‑word messages.  
2. **Empty‑token guard** before indexing `target[0]`.  
3. **Proper channel vs. nick check** using `||` and separating out operator masks.  
4. **Max‑targets check** (`ERR_TOOMANYTARGETS`) against your server’s limit.  
5. **Safe `msg` assembly** with a guard against `msg.empty()` before stripping `':'`.  
6. **`ERR_NOTEXTTOSEND`** if `msg` is empty after parsing.  
7. **Split loops**: channels, then masks (if any), then users, so errors and successes aren’t interleaved.  
8. **Null checks before dereference** of `chan` and `target`.  
9. **Single `findUser` call** per nick, returning a pointer so you don’t slice or lose state.  
10. **Permission checks** for no‑external‑msgs (operators & channel membership).  

This version should be robust, RFC‑compliant, and crash‑safe. Let me know if you need anything else!






