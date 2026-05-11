#ifndef IRC_REPLIES_HPP
#define IRC_REPLIES_HPP

enum class IRCReply
{
    // Welcome replies (sent after registration)
    RPL_WELCOME         = 001,  // Welcome to the Internet Relay Chat Network
    RPL_YOURHOST        = 002,  // Your host is <servername>, running version <version>
    RPL_CREATED         = 003,  // This server was created <date>
    RPL_MYINFO          = 004,  // <servername> <version> <available user modes> <available channel modes>

    // User replies
    RPL_AWAY            = 301,  // <nick> :<away message>
    RPL_UNAWAY          = 305,  // :You are no longer marked as being away
    RPL_NOWAWAY         = 306,  // :You have been marked as being away

    // WHOIS/WHO replies
    RPL_WHOISUSER       = 311,  // <nick> <user> <host> * :<real name>
    RPL_WHOISSERVER     = 312,  // <nick> <server> :<server info>
    RPL_WHOISOPERATOR   = 313,  // <nick> :is an IRC operator
    RPL_WHOISIDLE       = 317,  // <nick> <idle> <sign-on time> :seconds idle, signon time
    RPL_ENDOFWHOIS      = 318,  // <nick> :End of WHOIS list
    RPL_WHOISCHANNELS   = 319,  // <nick> :{[@|+]<channel> ...}

    // Channel replies
    RPL_NOTOPIC         = 331,  // <channel> :No topic is set
    RPL_TOPIC           = 332,  // <channel> :<topic>
    RPL_INVITING        = 341,  // <channel> <nick>
    RPL_NAMREPLY        = 353,  // <channel> :<nick> {<space><nick>}
    RPL_ENDOFNAMES      = 366,  // <channel> :End of NAMES list

    // Server replies
    RPL_MOTDSTART       = 375,  // :- <server> Message of the day -
    RPL_MOTD            = 372,  // :- <text>
    RPL_ENDOFMOTD       = 376,  // :End of MOTD command

    // Error replies
    ERR_NOSUCHNICK          = 401,  // <nickname> :No such nick/channel
    ERR_NOSUCHCHANNEL       = 403,  // <channel name> :No such channel
    ERR_CANNOTSENDTOCHAN    = 404,  // <channel name> :Cannot send to channel
    ERR_NORECIPIENT         = 411,  // :No recipient given (<command>)
    ERR_NOTEXTTOSEND        = 412,  // :No text to send
    ERR_UNKNOWNCOMMAND      = 421,  // <command> :Unknown command
    ERR_NOMOTD              = 422,  // :MOTD File is missing
    ERR_NONICKNAMEGIVEN     = 431,  // :No nickname given
    ERR_ERRONEUSNICKNAME    = 432,  // <nick> :Erroneous nickname
    ERR_NICKNAMEINUSE       = 433,  // <nick> :Nickname is already in use
    ERR_USERNOTINCHANNEL    = 441,  // <nick> <channel> :They aren't on that channel
    ERR_NOTONCHANNEL        = 442,  // <channel> :You're not on that channel
    ERR_USERONCHANNEL       = 443,  // <user> <channel> :is already on channel
    ERR_NOTREGISTERED       = 451,  // :You have not registered
    ERR_NEEDMOREPARAMS      = 461,  // <command> :Not enough parameters
    ERR_ALREADYREGISTERED   = 462,  // :Unauthorized command (already registered)
    ERR_PASSWDMISMATCH      = 464,  // :Password incorrect
    ERR_CHANNELISFULL       = 471,  // <channel> :Cannot join channel (+l)
    ERR_UNKNOWNMODE         = 472,  // <char> :is unknown mode char to me for <channel>
    ERR_INVITEONLYCHAN      = 473,  // <channel> :Cannot join channel (+i)
    ERR_BANNEDFROMCHAN      = 474,  // <channel> :Cannot join channel (+b)
    ERR_BADCHANNELKEY       = 475,  // <channel> :Cannot join channel (+k)
    ERR_CHANOPRIVSNEEDED    = 482,  // <channel> :You're not channel operator
};

#endif
