Prereqs
===

* Make sure Kali-pi or a similar OS is installed (make sure it allows for network monitoring)
* Enable monitoring mode
* Follow instructions under the `Prototype` folder




Qubie
===

*Qubie*, also known as the Poll Queue Monitor, is a part of the Free
& Fair suite of products. It is a compact polling place monitoring
system that attempts to estimate the wait time for casting a vote. It
can be used by jurisdictions to gather data about polling place usage
and performance, for use in allocating resources for future
elections. It can also optionally make wait time information available
on the Web in real time, so that voters can accurately gauge how long
the voting process will take them and plan accordingly (by choosing
among polling places, if they have multiple options; by voting at a
different time; or, in jurisdictions with early voting, by voting on a
different day). The system has two main components: a small
open-source hardware box running open-source software that monitors a
polling place for mobile device activity, and an optional application
that makes wait time estimates available online.

Correctness of the software components of the system, and privacy and
security guarantees with respect to the individually identifiable data
they detect, are critical aspects of this project.

Prototype Background
===

Prototype implementations of Qubie were developed as a part of the
DemTech Research Project at the IT University of Copenhagen in
2011-2015 and were tested at Danish polling places. This
implementation of Qubie is being developed completely from scratch,
independently of the original project.

Development Process and Methodology
===

*Qubie* is being developed using the Trust-by-Design (TBD) software 
engineering methodology. The initial prototype version, available in 
the "prototype" directory, uses a subset of this methodology.

The TBD methodology is documented in several papers published by Joe
Kiniry and his coauthors, available via http://www.kindsoftware.com/.
In particular, read "Secret Ninja Formal Methods" and "A 
Verification-centric Software Development Process for Java", both of
which were written with Dan Zimmerman.

In general, a system is comprised of:

* a top-level readme (like this one) that includes information about
  the system's purpose, examples of its use, fundamental concepts,
  system requirements, and background literature,

* a domain analysis and a detailed architecture specifications written
  in the [Extended Business Object Notation (EBON)] [3],

* where applicable, formal specifications written at the source code 
  level in one or more contract-based specification languages like 
  [Code Contracts] [1] (for .NET systems), the 
  [Java Modeling Language] [2] (for JVM systems), or the 
  [Executable ANSI/ISO C Specification Language (E-ACSL)] [4],

* protocol descriptions typically formally specified using abstract
  state machines (ASMs), petri nets, formally annotated collaboration
  diagrams, or other formal notations that have tool support for
  reasoning about such protocols,

* a hand-written set of (sub)system tests and an automatically
  generated set of unit tests (using, e.g., [PEX] [7] for .NET systems and
  [JMLUnitNG] [8] for JVM ones), including reports on the completeness
  and quality of these validation artifacts, and

* a set of evidence that the system fulfills its requirements, usually
  in the form of traceable artifacts from the requirements to other
  artifacts that validate that they are satisfied (e.g., test results,
  code reviews, formal proofs, etc.).

Requirements
===

What follows are the mandatory and secondary requirements imposed upon
*Qubie*.  Informal verification (in the traditional software
engineering sense) of these requirements is accomplished by several
means, including formal verification of properties of the system's
specification and implementation, as well as traceability from the
requirements to artifacts that validate that they are satisfied (e.g.,
system tests, code review, etc.).

Mandatory Requirements
==

* Must be able to detect the presence of voters in a polling place,
using radio emissions of their wireless devices as a proxy.
* Must be able to estimate the amount of time that each individual voter
spends in the polling place.
* Must maintain historical information about polling place occupancy for 
at least 14 hours of activity.
* Must be able to estimate the current wait time for voting based on 
the amounts of time that voters have spent in the polling place.
* Must be able to detect "outliers", such as poll workers and other individuals
who live and work in immediate proximity to the polling place, who are present
for long amounts of time but should not be considered in wait time 
calculations.
* Must not save any personally identifiable information about any individual 
(e.g., device MAC address) to non-volatile storage at any time.
* Must not allow any personally identifiable information about any individual 
(e.g., device MAC address) to leave the hardware device at any time, by any 
means.

Secondary Requirements
===

#### Usability:

* The system should be trivial to use for non-technical users (polling place 
workers), and ideally require only a single button press ("power on") to 
function as a data collector.

#### Efficiency:

* The system should be able to run for an entire day on battery power.

#### Scalability:

* The system should be able to handle a large number of voters
  (approximately 30,000 voters in a single polling place in a single day).

#### Security:

* The system should use proper security measures and cryptography to
  establish confidence that the system is secure.

#### Analysis:

* The system should be able to provide an analysis of polling place
  activity, including statistics about wait times and turnout.
* The system should have a public API for the media or any citizen to
  access (after the election).
* The system should be able to visualize the polling place activity.

Development Instructions
===

Development on the prototype version of *Qubie* is taking place on the 
Raspberry Pi platform. It is dependent on the following libraries, tools, 
and frameworks:

* [Python] [5]
* [Node.js] [6]
* [Pcapy] [7], for wireless packet inspection
* [Bleno] [8], for Bluetooth Low Energy communication

Source Code
===

The source code is currently under development for use on an embedded system with a minimal OS (e.g. rtos) and depends onf the following libraries, tools, and frameworks:

* [E-ACSL] [4]
* [libpcap] [9]
* [libsodium] [10]
 

[1]: http://research.microsoft.com/en-us/projects/contracts/  "Code Contracts library for .NET"

[2]: http://www.jmlspecs.org/  "Java Modeling Language (JML)"

[3]: http://bon-method.com/  "The Business Object Notation"

[4]: http://frama-c.com/ "The Executable ANSI/ISO C Specification Language"

[5]: http://www.python.org/ "Python"

[6]: https://nodejs.org/ "Node.js"

[7]: https://github.com/CoreSecurity/pcapy "Pcapy"

[8]: https://github.com/sandeepmistry/bleno "Bleno"

[9]: https://github.com/the-tcpdump-group/libpcap "libpcap"

[10]: https://github.com/jedisct1/libsodium "libsodium"

