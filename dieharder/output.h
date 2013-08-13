 /*
  * These are all binary flags, used to control precisely what comes out
  * of dieharder in its reports.  The vector of strings immediately
  * below it can be used to match up names to the power of two of each
  * entry.  Be sure to keep the two precisely in sync!
  */
 typedef enum {
   TDEFAULT = 0,
   THEADER = 1,
   TSHOW_RNG = 2,
   TLINE_HEADER = 4,
   TTEST_NAME = 8,
   TNTUPLE = 16,
   TTSAMPLES = 32,
   TPSAMPLES = 64,
   TPVALUES = 128,
   TASSESSMENT = 256,
   TPREFIX = 512,
   TDESCRIPTION = 1024,
   THISTOGRAM = 2048,
   TSEED = 4096,
   TRATE = 8192,
   TNUM = 16384,
   TNO_WHITE = 32768
 } Table;

#define TCNT 16

 /*
  * These should have a maximum length one can use in strncmp().
  * At the moment it looks like 10, but I think 16 is a safer
  * bet (and still safe from a buffer overwrite point of view)
  */
#define TLENGTH 16
 static char *table_fields[] __attribute__((unused)) = {
 "default",
 "header",
 "show_rng",
 "line_header",
 "test_name",
 "ntuple",
 "tsamples",
 "psamples",
 "pvalues",
 "assessment",
 "prefix",
 "description",
 "histogram",
 "seed",
 "rate",
 "show_num",
 "no_whitespace"
 };
