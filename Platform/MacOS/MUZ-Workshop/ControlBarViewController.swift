//
//  ControlBarViewController.swift
//  MUZ-Workshop
//
//  Created by Francis Pierot on 27/01/2019.
//  Copyright © 2019 Francis Pierot. All rights reserved.
//

import Cocoa

class ControlBarViewController: NSViewController {
	@IBOutlet weak var checkPanel: NSButton!
	@IBOutlet weak var listPanel: NSPopUpButton!
	@IBOutlet weak var buttonStep: NSButton!
	@IBAction func checkPanelClick(_ sender: Any) {
		listPanel.cell?.isEnabled = (checkPanel.state == .on);
	}
	@IBAction func listPanelChoice(_ sender: Any) {
	}
	@IBAction func buttonStepAction(_ sender: Any) {
	}
	override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
    }
    
}
