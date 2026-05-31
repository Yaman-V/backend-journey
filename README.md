# Backend Journey
A structured personal repository documenting my progress through backend engineering concepts in Java. This log covers core topics including REST API design, Spring Boot fundamentals, database integration, and clean software architecture.

**Primary Stack:** Java, Spring Boot, PostgreSQL, Docker
**Goal:** Develop production-grade backend systems and master enterprise-level architecture.

---

## Table of Contents
* [Week 00 — Foundation & Planning](#https://github.com/Yaman-V/backend-journey/tree/main#week-00---foundation--planning)
* [Week 01 — Spring Boot Fundamentals & REST APIs](#https://github.com/Yaman-V/backend-journey/tree/main#week-01--spring-boot-fundamentals--rest-apis)

---

## Week 00 - Foundation & Planning

### Goal
Establish the development environment, review core computer science fundamentals, and plan the roadmap for the backend journey.

### Execution
**Infrastructure & Tools**
* Configured IntelliJ IDEA Ultimate and JDK environment.

**Theory & Review**
* Audited core Object-Oriented Programming (OOP) concepts.
* Reviewed structural and behavioral Design Patterns.
* Studied OS 101 and CPU architecture.

**Projects**
* **Parallel_Sum:** Implemented multi-processing in C++. *(Moved to standalone repo: [Parallel_Sum](https://github.com/Yaman-V/Parallel_Sum))*
* **Library Management System:** Drafted Phase 01 Requirements. *(Status: Suspended to prioritize Spring Boot ecosystem).*

### Reflection
* **What went well:** Testing different workflows and establishing a baseline for my schedule.
* **Challenges:** Adapting to new university policies and calendar shifts which interrupted initial project timelines.
* **Gaps Identified:** Need to deepen understanding of the Java Collections Framework.
* **Takeaway:** This week served its purpose—clearing the runway, identifying what works for my schedule, and building momentum for the actual framework implementation next week.

---

## Week 01 - Spring Boot Fundamentals & REST APIs

### Goal
Transition from core Java to enterprise web development by initializing a Spring Boot application and building a fully functional, in-memory REST API.

### Execution
**Projects**
* **Task Manager API (Phase 1):** Built the Controller and Service layers for a complete CRUD (Create, Read, Update, Delete) application. 
* *(Note: This flagship project is maintained in a dedicated portfolio repository: https://github.com/Yaman-V/task-manager-api)*

**Theory & Implementation**
* Initialized the project via Spring Initializr (`pom.xml`, Maven, Spring Web).
* Implemented a layered MVC architecture (Model, Service, Controller).
* Enforced the Single Responsibility Principle by decoupling web routing from data management.
* Replaced standard UUIDs with a thread-safe `AtomicLong` for ID generation to mimic relational database sequencing.

### Concepts Mastered
* **Inversion of Control (IoC) & Dependency Injection (DI):** Moving away from the `new` keyword and utilizing Constructor Injection to manage Spring Beans.
* **Spring Web Annotations:** `@RestController`, `@Service`, `@GetMapping`, `@PostMapping`, `@PutMapping`, `@DeleteMapping`.
* **Data Mapping:** Using `@PathVariable` and `@RequestBody` to handle incoming JSON payloads via Jackson.
* **HTTP Status Control:** Wrapping standard POJOs in `ResponseEntity<T>` to accurately return `200 OK`, `201 Created`, `204 No Content`, and `404 Not Found`.
* **Safe Null Handling:** Utilizing `java.util.Optional` in the Service layer to prevent `NullPointerExceptions` when fetching data.

### Reflection
* **What went well:** Core Java knowledge (like standardizing Enums for status states and writing custom Lambda expressions for list filtering) translated perfectly into the Spring ecosystem.
* **Challenges:** Understanding how the Jackson JSON parser implicitly relies on default empty constructors to deserialize payloads.
* **Takeaway:** Frameworks can feel like "magic," but they are just Java under the hood. Forcing the data logic out of the controller and into a dedicated Singleton Service completely changed how I view application state and memory management.
