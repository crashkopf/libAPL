// log.c - logging facilities
/*

Logged data can't be written immediately, so it has to be buffered.
Each datum needs a text label, a pointer to a string in program memory


<dataset name="Engine">
	<datum name="T_intake" unit="deg C" type="int" />
	<datum name="T_exhaust" unit="deg C" type="int" />
	<datum name="P_manifold" unit="kPa" type="uint" />
	<datum name="Lambda" unit="lambda" type="double" />
</dataset>

<data name="Engine", time="11624">TWFuI,GlzIG,Rpc3R,pbmd1</data>
<data name="Engine", time="11625">TWFuI,GlzIG,Rpc3R,pbmd1</data>
<data name="Engine", time="11626">TWFuI,GlzIG,Rpc3R,pbmd1</data>
<data name="Engine", time="11627">TWFuI,GlzIG,Rpc3R,pbmd1</data>
<data name="Engine", time="11628">TWFuI,GlzIG,Rpc3R,pbmd1</data>
*/

typedef struct {
		
} logdev;

typedef struct {
	const char * label;
	void * value;
} datum;


void logdata ( const char * label, datum d ) {}
	
void logmsg ( const char * msg, unsigned level) {}
	
