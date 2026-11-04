import CoreGraphics
import Foundation
// Prints the id of the first on-screen window of the process whose name is given (default dosbox-x).
let name = CommandLine.arguments.count > 1 ? CommandLine.arguments[1] : "dosbox-x"
let list = CGWindowListCopyWindowInfo([.optionOnScreenOnly], kCGNullWindowID) as? [[String: Any]] ?? []
for w in list {
    if let owner = w[kCGWindowOwnerName as String] as? String, owner.lowercased().contains(name),
       let id = w[kCGWindowNumber as String] as? Int, let b = w[kCGWindowBounds as String] as? [String: Any],
       let h = b["Height"] as? Double, h > 100 { print(id); break }
}
