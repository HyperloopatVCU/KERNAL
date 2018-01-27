KERNEL_STRUCTURE

	The kernel, in this case, provides only an administrated application
space for device drivers. This administration entails registration of usable
memory space (for use by kernal functions) and some memory operation functions,
for ease of use by the programmers. THERE WILL BE NO SECURITY, so device
drivers may chose to ignore the built-in IO calls, but be aware that you can
crash the system by modifying certain things that the kernel relies on!

-------------------------------------------------------------------------------

STARTUP

There are a few ways to handle system startup:

1.)	Call a BIOS-like routine which probes and registers the known IO for
recognizable devices. This acts as a sort of index which the kernel uses to
setup device drivers and tell the RPi how things are connected.
	->	These 'indexes' are also called network trees, and are
		submitted to the RPi's known configuration. This configuration
		should be modifiable, and any changes should be uploaded to the
		parent Arduino so the kernel can use the devices properly.


2.)	Let the RPi handle the network configuration, which must be filled out
manually by a system operator. The Arduinos then wait until a connection has
been established with the RPi and must expect a network configuration to be
recieved with itself as the root of the tree. The kernel then uses this as its
basis for managing device drivers.

------------------------------------------------------------------------------ 

DATABASING

	In order to do its task properly, the kernel has to keep track of what
it wants to do and manage its resources efficiently. One way this can be
achieved is through a library of standard functions which keep track of and
administrate memory, IO, and other usable features. This may be split up into
more specific libraries, such as local memory, ADC inputs, clock usage, etc.

------------------------------------------------------------------------------

SCHEDULING

	The kernel is in charge of scheduling execution of the device drivers
so that the system either doesn't spend too much time waiting for a process to
complete or so we can be sure it has executed everything it needs to.
Essentially, this is either a timesharing system or the drivers are designed
specifically to manage cycles.

------------------------------------------------------------------------------

FORMAT
	KERNAL - Routine which uses its multiple functional libraries as a
standard for setting up and maintaining a system. This is the operating
system. Administrates te creation and execution of services, such as device
drivers.

	BIOS - Functional library used for communications. Supports buffering,
formatting, macro addressing, etc.

	REGISTRY - Functional library used to catalog machine resources and
data. Used mostly as a standard tree-like index. Supports searching.

	DATABASE - Functional library used primarily for memory management and
private area registration. Keeps track of how its being used.

	SCHEDULER - Functional library and routine used to set runtime
precedence (order) and register new 'main' execution vectors.


 vim:tw=78:ts=8:norl:
