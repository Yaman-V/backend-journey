package com.taskmanager.api.controller;

import com.taskmanager.api.model.Task;
import com.taskmanager.api.service.TaskService;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
public class TaskController {

    // 1. Declare it as a private final variable
    private final TaskService taskService;

    // 2. Pass it into the constructor.
    // Spring is smart enough to find its Service Bean and plug it in here automatically.
    public TaskController(TaskService taskService) {
        this.taskService = taskService;
    }

    @GetMapping("/tasks")
    public ResponseEntity<List<Task>> getAllTasks() {
        // 3. Now we use the injected instance, NO 'new' keyword!
        return ResponseEntity.ok(taskService.getAllTasks());
    }

    @GetMapping("/tasks/{id}")
    public ResponseEntity<Task> getTaskByID(@PathVariable Long id){
        Optional<Task> foundTask = taskService.getTaskByID(id);
        if (foundTask.isEmpty())
            return ResponseEntity.notFound().build();
        return ResponseEntity.ok(foundTask.get());
    }

    @PostMapping("/tasks")
    public ResponseEntity<Task> addTask(@RequestBody Task task){
        return ResponseEntity.status(HttpStatus.CREATED).body(taskService.addTask(task));
    }

    @PutMapping("/tasks/{id}")
    public ResponseEntity<Task>  updateTask(@PathVariable Long id ,@RequestBody Task updatedTask){
        Optional<Task> newTask =  taskService.updateTask(id , updatedTask);
        if (newTask.isEmpty())
            return ResponseEntity.notFound().build();
        return ResponseEntity.ok(newTask.get());

    }

    @DeleteMapping("/tasks/{id}")
    public ResponseEntity<Void> deleteTask(@PathVariable Long id){
         return taskService.deleteTask(id) ? ResponseEntity.noContent().build() : ResponseEntity.notFound().build();
    }
}