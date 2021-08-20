This is my effort to incorporate certain features of the Batocera EmulationStation UI into a standard RetroPie image. The main issues seem to be different locations for the config files, and running roms currently doesnt work either. 

_________________________________________________________________________________________________________________________________________________________________________________

Install guide:

First we gotta create the install option within retropie_setup.sh:

    sudo nano /home/pi/RetroPie-Setup/scriptmodules/supplementary/emulationstation-custom.sh
  
Copy and paste the following code:
     
    #!/usr/bin/env bash

    rp_module_id="emulationstation-custom"
    rp_module_desc="A modified version of EmulationStation."
    rp_module_section="core"
    rp_module_repo="git https://github.com/deep8787/batocera-emulationstation master"
    rp_module_flags="frontend"


    function depends_emulationstation-custom() {
      depends_emulationstation
    }

    function sources_emulationstation-custom() {
      sources_emulationstation
    }

    function build_emulationstation-custom() {
      build_emulationstation
    }

    function install_emulationstation-custom() {
      install_emulationstation
    }

    function configure_emulationstation-custom() {
      configure_emulationstation
    }

    function gui_emulationstation-custom() {
      gui_emulationstation
    }
  
Ctrl+X and press Y to save

Now go into the retropie_setup.sh and go to Manage packages, Core and you should see the new entry. Install via source.

This wont boot yet, you will need to make one more edit:

    sudo nano /opt/retropie/configs/all/autostart.sh
  
And replace 

    emulationstation #auto

with the following 2 lines:

    cd /opt/retropie/supplementary/emulationstation-custom
    ./emulationstation

Ctrl+X and press Y to save

Reboot and it should load up into the new EmulationStation. FYI this is a work in progress, most of the Start menu items dont work yet. Nor does booting roms. All in good time, if anyone wants to help, they are more then welcome to!
