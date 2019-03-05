//
//  LeftPanelSplitViewController.swift
//  MUZ-Workshop-brouillon
//
//  Created by Francis Pierot on 04/02/2019.
//  Copyright © 2019 Francis Pierot. All rights reserved.
//

import Cocoa

class LeftPanelSplitViewController: NSSplitViewController {

	/** outlet of the outline view for the project files */
	@IBOutlet weak var filesView: NSOutlineView!


	override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
    }
    
}
