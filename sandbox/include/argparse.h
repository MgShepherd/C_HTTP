#ifndef _ARGPARSE_H_
#define _ARGPARSE_H_

typedef enum { MODE_SERVER, MODE_CLIENT, MODE_HELP } Mode;

typedef struct {
  Mode mode;
} CmdArgs;

CmdArgs parse_cmd_args(int argc, char **argv);
void display_usage_text();

#endif // !_ARGPARSE_H_
