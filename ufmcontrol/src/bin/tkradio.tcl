#!/bin/sh
#
# Copyright (c) 2005-2013 Nicola Vitale <nivit@users.sourceforge.net>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# $Id: tkradio.tcl,v 1.6 2009/04/09 08:26:35 nivit Exp $
# \
exec wish "$0"

wm protocol . WM_DELETE_WINDOW powerOff
wm resizable . false false
wm title . tkradio

###########################
# Native Language Support #
###########################
package require msgcat
::msgcat::mcload [join [list $env(LANG) tkradio] {/}]

# set some default values
array set config {
    cmdDecFreq		{ufmcontrol -s -}
    cmdGetFreq		{ufmcontrol -L get}
    cmdIncFreq		{ufmcontrol -s +}
    cmdPlayRadio	{ufmcontrol -s -L play}
    cmdSetFreq		{ufmcontrol -s -L}
    cmdSetJFreq		{ufmcontrol -s -L -j}
    cmdStereoMode	{ufmcontrol -L stereo}
    cmdStopRadio	{ufmcontrol -s -L stop}
    japaneseFreq	{no}
    lastStation		{88.10}
    locale		{en_EN.ISO8859-1}
    meterBgColor	{black}
    meterFgColor	{red}
    mixerDevice		{/dev/mixer0}
    mixerInputJack	{mic}
    mixerRecLevel	{75}
    mixerVolLevel	{75}
    radioDevice		{/dev/ufm0}
    saveLastStation	{yes}
    saveVolLevel	{yes}
    station1 		{107.05}
    station2		{100.20}
    station3		{99.50}
    station4		{99.80}
    turnOffRadio	{yes}
}

variable volume
variable curFreq
variable stereoMode
variable bandLowLimit bandHighLimit
array set stationList {}
# debug: error=question=0, warning=1, info=2
set debug 1
set sysConfFile /usr/local/lib/ufmcontrol/share/xufmrc
set confDir $env(HOME)/.config/tkradio
set usrConfFile $confDir/config

#######################
# print a message box #
#######################
proc dbg {level msg} {
    global debug
    variable Ret 0 Lvl 0
    set title [::msgcat::mc "$level"]
    switch -exact -- $level {
	# NLS("error")
        error	 {
            set Lvl 0
        }
	# NLS("question")
        question {
            set Lvl 0
        }
	# NLS("warning")
        warning	 {
            set Lvl 1
        }
	# NLS("info")
        info	 {
            set Lvl 2
        }
        default {
            set Lvl 0
        }
    }
    if {$Lvl <= $debug} {
        set Ret [tk_messageBox -message $msg -type ok \
                -icon $level -title "tkradio: $title"]
    }
    return $Ret
}

###########################
# read configuration file #
###########################
proc loadConfig {confFile} {
    # open file
    if { [catch {set chid [open $confFile r]} error_msg] } {
	dbg error "Could not open $confFile for reading\n$error_msg"
        exit 1
    } else {
        dbg info "Opened $confFile with success.\n"
        # read file
        while {![eof $chid]} {
            gets $chid line
            # lines which start with # are comments
            if { [regexp -expanded -- {([^\#]+)=(.*)} $line tot var val] } {
                global config
                # set new values
                foreach n [lsort [array names config]] {
                    set name [string trim $var]
                    set value [string trim $val]
                    if {$n == $name} then {
                        set config($n) $value
                    }
                }
            }
        }
        close $chid
    }
    return 0
}

######################
# save configuration #
######################
proc saveConfig {confFile} {
    if { [catch {set chid [open $confFile w]} error_msg] } {
        dbg error "Could not open $confFile for writing.\n"
        exit 1
    } else {
        dbg info "Opened $confFile with success.\n"
        # write an header
        puts $chid "#\n# tkradio configuration file\n#\n"
        # read options
        global config
        foreach {option} [lsort [array names config]] {
            # eventually skip lastStation option
            if {($config(saveLastStation) == {no}) && \
                        ($option == {lastStation})} continue
            # eventually skip mixerVolLevel option
            if {($config(saveVolLevel) == {no}) && \
                        ($option == {mixerVolLevel})} continue
            puts $chid "$option = $config($option)"
        }
        close $chid
    }
    return 0
}

###########################
# Radio control functions #
###########################
proc setjfreq {yesno} { # set japanese frequencies
    global config bandLowLimit bandHighLimit
    set config(japaneseFreq) $yesno
    if {$yesno == "no"} {
        set bandLowLimit 88.00
        set bandHighLimit 108.00
    } else {
        set bandLowLimit 76.00
        set bandHighLimit 91.00
    }
    return
}

proc powerOff {} {
    global config usrConfFile curFreq volume
    if {$config(turnOffRadio) == yes} {
        catch {eval exec $config(cmdStopRadio)}
    }
    # set current values of volume and frequency in config
    array set config [list lastStation $curFreq]
    array set config [list mixerVolLevel $volume]
    if {[file exists $usrConfFile]} {
        saveConfig $usrConfFile
    }
    exit 0
}

###########################
# Mixer control functions #
###########################
proc getVolume {} {
    global config
    if {![file exists $config(mixerDevice)]} {
        dbg warning "Mixer device ($config(mixerDevice))\nnot configured!"
        return 0
    } else {
        regsub -- {:} [exec mixer -f $config(mixerDevice) -s $config(mixerInputJack)] { } result
        return [lindex $result end]
    }
}

proc setVolume {level} {
    global config volume
    if {[file exists $config(mixerDevice)]} then {
        if {($level >= 0) && ($level <= 100)} {
            catch {exec mixer -f $config(mixerDevice) $config(mixerInputJack) $level]}
            set volume $level
        }
    }
    return
}

#####################
# crontab functions #
#####################

###########
# tkradio #
###########
proc setPrefs {} {
    dbg warning [::msgcat::mc "Not yet implemented."]
    return
}

proc setFreq {f} {
    # upvar?
    global curFreq config
    global bandLowLimit
    global bandHighLimit
    if {$config(japaneseFreq) == yes} {
        set cmd $config(cmdSetJFreq)
    } else {
        set cmd $config(cmdSetFreq)
    }
    if {($f > $bandHighLimit) || ($f < $bandLowLimit)} {
        if {($config(lastStation) > $bandHighLimit) \
                    || ($config(lastStation)  < $bandLowLimit)} {
            catch {eval exec $cmd $bandLowLimit)}
            set curFreq $bandLowLimit
        } else {
            catch {eval exec $cmd $config(lastStation)}
            set curFreq $config(lastStation)
        }
    } else {
        catch {eval exec $cmd $f)}
        set curFreq $f
    }
    return
}

proc getStereoMode {} {
    global config stereoMode
    if {[catch {eval exec $config(cmdStereoMode)}]} {
        set stereoMode 0
    } else {
        set stereoMode 1
    }
    return
}

proc appInit {} {
    global sysConfFile usrConfFile confDir config
    # load system configuration file
    if {[file exists $sysConfFile]} {
        loadConfig $sysConfFile
    }
    if {![info exists env(HOME)]} {
        set env(HOME) ~
    }
    # create the directory $HOME/.config/tkradio
    if {![file isdirectory $confDir]} {
	if {[file isfile $confDir]} {
	    file rename -force $confDir "$confDir.old"
	}
        file mkdir $confDir
    }
    # create the directory $HOME/.config/tkradio/crontab
    set crontabDir [join [list $confDir crontab] /]
    if {![file isdirectory $crontabDir]} {
	if {[file isfile $crontabDir]} {
	    file rename -force $crontabDir "$crontabDir.old"
	}
	file mkdir $crontabDir
    }
    # load/save user configuration
    if {[file exists $usrConfFile]} {
        loadConfig $usrConfFile
    } else {
        saveConfig $usrConfFile
    }
    # check mixer device and permissions
    if {![file exists $config(mixerDevice)]} {
        dbg error "Device $config(mixerDevice)\nnot configured!"
        exit 1
    } elseif {![file readable $config(mixerDevice)]} {
        dbg error "No read permission\non $config(mixerDevice)!"
        exit 1
    } elseif {![file writable $config(mixerDevice)]} {
        dbg error "No write permission\non $config(mixerDevice)!"
        exit 1
    } elseif {[catch {set chid [open $config(mixerDevice) r+]} error_msg]} {
        dbg error "$error_msg"
        exit 1
    }
    # check radio device and permissions
    if {![file exists $config(radioDevice)]} {
        dbg error "Device $config(radioDevice)\nnot configured!"
        exit 1
    } elseif {![file readable $config(radioDevice)]} {
        dbg error "No read permission\non $config(radioDevice)!"
        exit 1
    } elseif {![file writable $config(radioDevice)]} {
        dbg error "No write permission\non $config(radioDevice)!"
        exit 1
    } elseif {[catch {set chid [open $config(radioDevice) r+]} error_msg]} {
        dbg error "$error_msg"
        exit 1
    }
    close $chid
    setjfreq $config(japaneseFreq)
    # Set an initial value for frequency
    if {$config(saveLastStation) == {yes}} {
        set GotFreq $config(lastStation)
    } else {
        catch {eval exec $config(cmdGetFreq)} GotFreq
        if {$GotFreq == "0.0"} {
            set GotFreq $config(station1)
        }
    }
    setFreq $GotFreq
    # Set an initial value for volume
    if {$config(saveVolLevel) == {yes}} {
        set GotVol $config(mixerVolLevel)
    } else {
        catch {getVolume} GotVol
    }
    setVolume $GotVol
    # Set an source/initial value for mixer.rec mixer.mic/line
    catch {exec mixer -f $config(mixerDevice) =rec $config(mixerInputJack)}
    catch {exec mixer -f $config(mixerDevice) rec $config(mixerRecLevel)}
    catch {exec mixer -f $config(mixerDevice) $config(mixerInputJack) $config(mixerVolLevel)}
    if { [catch {package require -exact snack 2.2} error_msg] } {
        dbg error "$error_msg\nSee http://www.speech.kth.se/~kare/\n"
        exit 1
    } else {
        # create meter/sound
        ::snack::sound s
        ::snack::mixer devices
        ::snack::mixer input $config(mixerInputJack)
	# activate meter
	meterOn
    }
    return
}

proc playRadio {} {
    global config
    catch {eval exec $config(cmdPlayRadio)}
    return
}

proc stopRadio {} {
    global config
    catch {eval exec $config(cmdStopRadio)}
    return
}

proc incFreq {} {
    global config curFreq bandHighLimit stereoMode
    if {[expr {$curFreq + {0.05}}] <= $bandHighLimit} {
        catch {eval exec $config(cmdIncFreq)}
        set curFreq [eval exec $config(cmdGetFreq)]
    }
}

proc decFreq {} {
    global config curFreq bandLowLimit stereoMode
    if {[expr {$curFreq - {0.05}}] >= $bandLowLimit} {
        catch {eval exec $config(cmdDecFreq)}
        set curFreq [eval exec $config(cmdGetFreq)]
    }
}

# Meter functions/data, written by Kåre Sjölander
# snack package - http://www.speech.kth.se/snack/
array set meterMap {
    0 2
    1 3
    2 4
    3 5
    4 7
    5 9
    6 12
    7 15
    8 19
    9 23
    10 28
    11 34
    12 41
    13 49
    14 56
    15 63
}

proc meterOn {} {
    s record
    after 100 meterDraw
    return
}

proc meterDraw {} {
    global optionalFrame
    if {[s length] > 129} {
        set spec [s dBPower -fftlen 128 -windowlength 128]
        s length 0
        for {set i 0} {$i < 16} {incr i} {
            set val [lindex $spec $::meterMap($i)]
            $optionalFrame.meter coords c$i [expr {10*($i-2)}] 0 [expr {10*($i-2)+9}] \
                    [expr {100-1.4*($val+100)}]
        }
    }
    if {![::snack::audio active]} return
    after 100 meterDraw
}

# end Meter section

appInit

# create widgets
set displayFrame [frame .displayFrame  -relief ridge -borderwidth 3]
set buttonsFrame [frame .buttonsFrame]
set optionalFrame [frame .optionalFrame]

# displayFrame.volumeSlider
set volumeSlider [scale $displayFrame.volumeSlider \
        -orient horizontal \
        -command {setVolume} \
        -digits 3 \
        -from 10 \
        -label [::msgcat::mc {Volume}] \
        -length 350 \
        -resolution 5 \
        -showvalue false \
        -sliderlength 40 \
        -tickinterval 10 \
        -to 100 \
        -variable volume
]

# displayFrame.frequencySlider
set frequencySlider [scale $displayFrame.frequencySlider \
        -orient horizontal \
        -command {setFreq} \
        -digits 5 \
        -from $bandLowLimit \
        -label [::msgcat::mc {Frequency / MHz}] \
        -length 350 \
        -resolution 0.01 \
        -showvalue true \
        -sliderlength 40 \
        -tickinterval 5 \
        -to $bandHighLimit \
        -variable curFreq
        ]

# buttonsFrame
set playButton [button $buttonsFrame.playButton \
        -text [::msgcat::mc {play}] -relief raised \
        -command {playRadio; meterOn} \
        ]
set stopButton [button $buttonsFrame.stopButton \
        -text [::msgcat::mc {stop}] -relief raised \
        -command {eval exec $config(cmdStopRadio); s stop} \
        ]
set powerButton [button $buttonsFrame.powerButton \
        -text [::msgcat::mc {exit}] -relief raised \
        -command {powerOff} \
        ]
set decVolButton [button $buttonsFrame.decVolButton \
        -text [::msgcat::mc {vol-}] -relief raised \
        -command {setVolume [expr {$volume - 5}]} \
        ]
set incVolButton [button $buttonsFrame.incVolButton \
        -text [::msgcat::mc {vol+}] -relief raised \
        -command {setVolume [expr {$volume + 5}]} \
        ]
set decFreqButton [button $buttonsFrame.decFreqButton \
        -text [::msgcat::mc {freq-}] -relief raised \
        -command {decFreq; meterOn} \
        ]
set incFreqButton [button $buttonsFrame.incFreqButton \
        -text [::msgcat::mc {freq+}] -relief raised \
        -command {incFreq; meterOn} \
        ]
set setPrefsButton [button $optionalFrame.setPrefsButton \
        -text [::msgcat::mc {Settings}] -relief raised \
        -command {setPrefs} \
        ]

# meter canvas
set meterCanvas [canvas $optionalFrame.meter -width 120 -height 100]

for {set i 0} {$i<16} {incr i} {
    global config
    set meterBgColor $config(meterBgColor)
    set meterFgColor $config(meterFgColor)
    $meterCanvas create rect [expr {10*$i}] 50 [expr {10*$i+10}] 100  -fill $meterFgColor -outline ""
    $meterCanvas create rect [expr {10*$i}] 20 [expr {10*$i+10}] 50 -fill $meterFgColor -outline ""
    $meterCanvas create rect [expr {10*$i}] 0  [expr {10*$i+10}] 20 -fill $meterFgColor -outline ""
    $meterCanvas create rect [expr {10*$i}] 0  [expr {10*$i+10}] 100 -fill $meterBgColor -tag c$i
}
for {set i 0} {$i<17} {incr i} {
    $meterCanvas create line 0 [expr {6*$i}] 140 [expr {6*$i}] -width 3
    $meterCanvas create line [expr {10*$i}] 0 [expr {10*$i}] 100 -width 5
}

# pack all widgets
pack $decVolButton -fill x -padx 5 -pady 5 -side left -expand true
pack $incVolButton -fill x -padx 5 -pady 5 -side left -expand true
pack $decFreqButton -fill x -padx 5 -pady 5 -side left -expand true
pack $incFreqButton -fill x -padx 5 -pady 5 -side left -expand true
pack $setPrefsButton -fill x -padx 5 -pady 5 -side bottom -expand true
pack $playButton -fill x -padx 5 -pady 5 -side left -expand true
pack $stopButton -fill x -padx 5 -pady 5 -side left -expand true
pack $powerButton -fill x -padx 5 -pady 5 -side left -expand true

pack $frequencySlider -expand true -fill x -fill y
pack $volumeSlider -expand true -fill x -fill y
pack $buttonsFrame -side bottom -expand true -fill y -fill x
pack $displayFrame -side left -expand true -fill x -fill y
pack $meterCanvas -side bottom -expand true -fill x -fill y
pack $optionalFrame -side top -expand true -fill x -fill y
