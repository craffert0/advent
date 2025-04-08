#! /usr/bin/swift

// https://adventofcode.com/2015/day/7

import Foundation

enum Calc {
    case And(String, String)
    case Or(String, String)
    case LShift(String, String)
    case RShift(String, String)
    case Not(String)
    case Value(String)

    var value: UInt16 {
        switch self {
        case let .And(x, y):
            lookup(x) & lookup(y)
        case let .Or(x, y):
            lookup(x) | lookup(y)
        case let .LShift(x, y):
            lookup(x) << lookup(y)
        case let .RShift(x, y):
            lookup(x) >> lookup(y)
        case let .Not(x):
            ~lookup(x)
        case let .Value(x):
            lookup(x)
        }
    }

    init(count: Int, s0: String, s1: String, s2: String) {
        self = switch count {
        case 5:
            switch s1 {
            case "AND": .And(s0, s2)
            case "OR": .Or(s0, s2)
            case "LSHIFT": .LShift(s0, s2)
            default: .RShift(s0, s2)
            }
        case 4: .Not(s1)
        default: .Value(s0)
        }
    }
}

class Node {
    let calc: Calc
    var cached_: UInt16?
    var value: UInt16 {
        let v = cached_ ?? calc.value
        cached_ = v
        return v
    }

    init(_ calc: Calc) {
        self.calc = calc
    }
}

var nodes: [String: Node] = [:]

func lookup(_ s: String) -> UInt16 {
    nodes[s]?.value ?? UInt16(s)!
}

for line in String(data: FileManager.default.contents(atPath: "input.txt")!,
                   encoding: .utf8)!.split(separator: "\n")
{
    let s = line.split(separator: " ")
    let name = String(s.last!)
    let calc = Calc(count: s.count, s0: String(s[0]), s1: String(s[1]),
                    s2: String(s[2]))
    nodes[name] = Node(calc)
}

for k in nodes.keys.sorted() {
    print(k, nodes[k]!.value)
}
