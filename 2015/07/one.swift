#! /usr/bin/swift

// https://adventofcode.com/2015/day/7

import Foundation

protocol Calc {
    var value: UInt16 { get }
}

class Value: Calc {
    let value_: String

    init(_ value: String) {
        self.value_ = value
    }

    var value: UInt16 {
        get {
            return lookup(value_)
        }
    }
}

class And: Calc {
    let lhs: String
    let rhs: String

    init(_ lhs: String, _ rhs: String) {
        self.lhs = lhs
        self.rhs = rhs
    }

    var value: UInt16 {
        get {
            return lookup(lhs) & lookup(rhs)
        }
    }
}

class Or: Calc {
    let lhs: String
    let rhs: String

    init(_ lhs: String, _ rhs: String) {
        self.lhs = lhs
        self.rhs = rhs
    }

    var value: UInt16 {
        get {
            return lookup(lhs) | lookup(rhs)
        }
    }
}

class LShift: Calc {
    let lhs: String
    let rhs: String

    init(_ lhs: String, _ rhs: String) {
        self.lhs = lhs
        self.rhs = rhs
    }

    var value: UInt16 {
        get {
            return (lookup(lhs) << lookup(rhs))
        }
    }
}

class RShift: Calc {
    let lhs: String
    let rhs: String

    init(_ lhs: String, _ rhs: String) {
        self.lhs = lhs
        self.rhs = rhs
    }

    var value: UInt16 {
        get {
            return (lookup(lhs) >> lookup(rhs))
        }
    }
}

class Not: Calc {
    let rhs: String

    init(_ rhs: String) {
        self.rhs = rhs
    }

    var value: UInt16 {
        get {
            return (~lookup(rhs))
        }
    }
}

class Node {
    let calc: Calc
    var cached_: UInt16?
    var value: UInt16 {
        get {
            let v = cached_ ?? calc.value
            cached_ = v
            return v
        }
    }

    init(_ calc: Calc) {
        self.calc = calc
    }
}

var nodes: [String: Node] = [:]

func lookup(_ s: String) -> UInt16{
    if let n = nodes[s] {
        return n.value
    } else {
        return UInt16(s)!
    }
}

for line in String(data: FileManager.default.contents(atPath: "input.txt")!,
                   encoding: .utf8)!.split(separator: "\n") {
    let s = line.split(separator: " ")
    let name = String(s.last!)
    let s0 = String(s[0])
    let s1 = String(s[1])
    let s2 = String(s[2])
    switch s.count {
    case 5:
        let op = s[1]
        if op == "AND" {
            nodes[name] = Node(And(s0, s2))
        } else if s1 == "OR" {
            nodes[name] = Node(Or(s0, s2))
        } else if s1 == "LSHIFT" {
            nodes[name] = Node(LShift(s0, s2))
        } else { // if s1 == "RSHIFT"
            nodes[name] = Node(RShift(s0, s2))
        }
    case 4:
        nodes[name] = Node(Not(s1))
    default: // case 3
        nodes[name] = Node(Value(s0))
    }
}

for k in nodes.keys.sorted() {
    print(k, nodes[k]!.value)
}
