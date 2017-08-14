#include "raytracer.h"

int main(int argc, char *argv[])
{
  int sel = 7;
  stage stg = get_stage(sel);
  render(stdout,stg);
  stage_show(stderr,stg);

  ol_free(stg.s.objects);

  return 0;
}

