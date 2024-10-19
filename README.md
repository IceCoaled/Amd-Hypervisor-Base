Hypervisor Virtualization Research

Overview

This project focuses on exploring hypervisor virtualization, specifically targeting AMD processors. It delves into the implementation, optimization, and detection methods of hypervisor-level virtualization, with emphasis on security mechanisms like System Virtual Machine Extensions (SVM) and how anti-cheat systems detect virtualization.

![Debug Log Screenshot](Screenshot 2024-10-19 141743.png)

The screenshot above shows successful steps during hypervisor initialization, including MSR Map Setup, Processor Virtualization, and Hypervisor Started messages. This indicates that our custom hypervisor is functioning correctly, passing key setup stages.

Background Research

AMD Programming References:

AMD SVM Reference (24594)

AMD Processor Reference (24593)

Anti-Cheat Detection for Vm exits:

How Anti-Cheats Detect System Emulation by Secret Club. This article discusses the techniques anti-cheat solutions use to detect system emulation, which is highly relevant to bypassing anti-virtualization detection when running the hypervisor.

Open Source Projects for Reference:

SimpleSvm by Tandasat — A project demonstrating the basics of AMD SVM implementation in a simple hypervisor.

HV by Jonomango — Another open-source hypervisor project that was used as inspiration for certain parts of our implementation.

Project Highlights

Driver Setup and Initialization: The project successfully initializes the driver entry points, indicating module and kernel list success. The setup includes nested pages and MSR map configurations.

MSR and Nested Pages Setup: Custom MSR (Model Specific Register) mappings were implemented, allowing efficient communication between the guest and hypervisor.

Features

Uses deferred procedure calls to virtualize processors.

I leverage a lot of C++ stuff in this driver even went the hard route of making the main hypervisor a class.

SVM-Based Virtualization: Leveraging AMD's SVM technology to create a hypervisor that can virtualize multiple processors with full control over virtualization settings.

Nested Pages Support: The hypervisor enables Nested Page Table (NPT) support to provide better performance in memory management and isolation.

Detection Evasion: Several methods are being researched to bypass anti-cheat detections of emulation or virtualization, ensuring minimal detection.

Goals

Optimization: Improve the hypervisor's performance and reduce overhead in nested and non-nested environments.

Security: Develop mechanisms to protect the hypervisor from being detected by anti-cheat software.

Contribution: Contribute back findings to the open-source community, adding to current hypervisor research.

Fix: driver unload function. do more testing to make sure it exits cleanly in any unsuccessful scenario's.

Getting Started

To try this hypervisor:

Clone the repository from GitHub [(link placeholder)](https://github.com/IceCoaled/Amd-Hypervisor-Base).

Compile the kernel driver using Visual Studio with WDK (Windows Driver Kit).

Ensure your system has SVM enabled in BIOS and that Hyper-V is either disabled or properly configured.

Load the driver and verify initialization logs similar to the screenshot above.

this works on vmware very clean, and bare metal, i cant get it to work good on hyper-v.

Credits

Contributors: This project was made possible with insights from community contributions, including the SimpleSvm and HV open-source hypervisor examples.

Special Thanks: To Secret Club for their research on anti-cheat virtualization detection, which provided valuable insights.

License

This project is licensed under the MIT License - see the LICENSE file for details.

Feel free to explore, experiment, and contribute to furthering hypervisor research!

