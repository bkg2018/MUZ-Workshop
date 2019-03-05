//
//  ViewController.swift
//  OutlineView
//
//  Created by Francis Pierot on 04/02/2019.
//  Copyright © 2019 Francis Pierot. All rights reserved.
//

import Cocoa

class ViewController: NSViewController {
	var creatures = [Creatures]();

	@IBOutlet weak var outlineView: NSOutlineView!
	override func viewDidLoad() {
		super.viewDidLoad()

		let creatureDict = ["Animals": ["Cat", "Dog", "Horse"],
							"Birds":   ["Eagle", "Hawk"],
							"Fish":    ["Cod", "Mackeral", "Salmon", "Tilapia"]]
		for (type, things) in creatureDict {
			let aCreatureList = Creatures(type: type, things: things)
			creatures.append(aCreatureList)
		}

		// this view controller is also the data source for its outline view
		outlineView.dataSource = self
		// and it is also the delegate for display
		outlineView.delegate = self
	}

	override var representedObject: Any? {
		didSet {
		// Update the view, if already loaded.
		}
	}
}

// extend the view conroller as a data source for an outline view
extension ViewController : NSOutlineViewDataSource {

	// Returns the child of an item, or a root node if item is null
	func outlineView(_ outlineView: NSOutlineView, child index: Int, ofItem item: Any?) -> Any {
		if let creatures = item as? Creatures {
			return creatures.things[index]
		}
		// return a root node
		return creatures[index]
	}

	// Tells if an item can be expanded or not
	func outlineView(_ outlineView: NSOutlineView, isItemExpandable item: Any) -> Bool {
		if let creatures = item as? Creatures {
			return creatures.things.count > 0
		}
		return false;
	}

	// Returns the number of children in an item
	func outlineView(_ outlineView: NSOutlineView, numberOfChildrenOfItem item: Any?) -> Int {
		if let creatures = item as? Creatures {
			return creatures.things.count
		}
		return creatures.count
	}

}

// extend the view controller as a delegate for the outline view
extension ViewController : NSOutlineViewDelegate {

	// Implement the view
	func outlineView(_ outlineView: NSOutlineView, viewFor tableColumn: NSTableColumn?, item: Any) -> NSView? {
		var text = ""
		if let creatures = item as? Creatures {
			text = creatures.type
		} else {
			text = item as! String
		}
		let tableCell = outlineView.makeView(withIdentifier: NSUserInterfaceItemIdentifier(rawValue: "fileCell"), owner: self) as! NSTableCellView
		tableCell.textField!.stringValue = text
		return tableCell
	}
}


