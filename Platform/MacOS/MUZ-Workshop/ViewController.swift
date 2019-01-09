//
//  ViewController.swift
//  MUZ-Workshop
//
//  Created by Francis Pierot on 26/11/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

import Cocoa

class ViewController: NSViewController {
	
	//MARK: Properties
	
	@IBOutlet weak var registerPC: NSTextField!
	@IBOutlet weak var registerAF: NSTextField!
	@IBOutlet weak var registerBC: NSTextField!
	@IBOutlet weak var registerDE: NSTextField!
	@IBOutlet weak var registerHL: NSTextField!
	@IBOutlet weak var registerIX: NSTextField!
	@IBOutlet weak var registerIY: NSTextField!
	@IBOutlet var buttonStep: NSButton!
	
	
	
	//MARK: MUZ bridge instance

	var bridge = MuzBridge()

	// MARK: Functions
	override func viewWillAppear() {
		self.bridge.start()
	}
	
	override func viewDidLoad() {
		super.viewDidLoad()
		// assemble
		//self.bridge.assemble()
	}

	@IBAction func hitStep(_ sender: Any) {
		self.bridge.step()
		let reg = self.bridge.registers()
		registerPC.stringValue = String(format:"%04X", reg.pc)
		registerAF.stringValue = String(format:"%04X", reg.af)
		registerBC.stringValue = String(format:"%04X", reg.bc)
		registerDE.stringValue = String(format:"%04X", reg.de)
		registerHL.stringValue = String(format:"%04X", reg.hl)
		registerIX.stringValue = String(format:"%04X", reg.ix)
		registerIY.stringValue = String(format:"%04X", reg.iy)
	}
	override var representedObject: Any? {
		didSet {
			
		}
	}


}

